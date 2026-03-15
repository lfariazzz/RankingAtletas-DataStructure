"""
Geração de Gráficos — Análise de Desempenho
Ranking de Atletas: Listas, Ordenação e Análise de Desempenho
"""

import csv
import os
import math
from collections import defaultdict

# ── Saída ────────────────────────────────────────────────────────────────────
OUTPUT_DIR = "graficos"
os.makedirs(OUTPUT_DIR, exist_ok=True)

# ── Leitura do CSV ────────────────────────────────────────────────────────────
def ler_csv(path):
    dados = []
    # Tentamos abrir como utf-16 primeiro, já que o erro apontou 0xff
    # Se falhar, ele tenta utf-8-sig (que ignora o BOM do UTF-8)
    try:
        encoding_to_try = 'utf-16'
        with open(path, newline="", encoding=encoding_to_try) as f:
            reader = csv.DictReader(f)
            # Forçamos a leitura da primeira linha para testar o encoding
            test_row = next(reader) 
            f.seek(0) # Volta pro começo se funcionou
            
    except (UnicodeError, StopIteration):
        encoding_to_try = 'utf-8-sig'

    with open(path, newline="", encoding=encoding_to_try) as f:
        reader = csv.DictReader(f)
        for row in reader:
            try:
                row["n"]                  = int(row["n"])
                row["media_tempo_ms"]     = float(row["media_tempo_ms"])
                row["media_tempo_ns"]     = float(row["media_tempo_ns"])
                row["media_comparacoes"]  = float(row["media_comparacoes"])
                row["desvio_ns"]          = float(row["desvio_ns"])
                dados.append(row)
            except (KeyError, ValueError) as e:
                print(f"⚠️ Pulando linha com erro ou cabeçalho inválido: {e}")
                continue
    return dados

# ── SVG helpers ───────────────────────────────────────────────────────────────
CORES = {
    "BubbleSort":    "#E24B4A",
    "InsertionSort": "#EF9F27",
    "SelectionSort": "#1D9E75",
    "QuickSort":     "#378ADD",
    "MergeSort":     "#7F77DD",
}

NOMES_ENTRADA = {
    "Aleatorio":            "Aleatório (caso médio)",
    "Ordenado":             "Ordenado (melhor caso)",
    "InversamenteOrdenado": "Inv. Ordenado (pior caso)",
}

def fmt_ms(v):
    if v < 0.001:
        return f"{v*1000:.4f} µs"
    if v < 1:
        return f"{v:.4f} ms"
    return f"{v:.2f} ms"

def linear_scale(values, y_min_px, y_max_px):
    """Mapeia valores para coordenadas SVG (y cresce para baixo)."""
    vmin, vmax = min(values), max(values)
    if vmax == vmin:
        vmax = vmin + 1
    def scale(v):
        ratio = (v - vmin) / (vmax - vmin)
        return y_max_px - ratio * (y_max_px - y_min_px)
    return scale, vmin, vmax

def log_scale(values, y_min_px, y_max_px):
    """Escala logarítmica para valores muito diferentes."""
    log_vals = [math.log10(max(v, 0.0001)) for v in values]
    lmin, lmax = min(log_vals), max(log_vals)
    if lmax == lmin:
        lmax = lmin + 1
    def scale(v):
        lv = math.log10(max(v, 0.0001))
        ratio = (lv - lmin) / (lmax - lmin)
        return y_max_px - ratio * (y_max_px - y_min_px)
    return scale, lmin, lmax

def salvar_svg(conteudo, nome):
    path = os.path.join(OUTPUT_DIR, nome)
    with open(path, "w", encoding="utf-8") as f:
        f.write(conteudo)
    print(f"  Salvo: {path}")

# ── Componentes SVG ───────────────────────────────────────────────────────────
def svg_header(W, H, titulo, subtitulo=""):
    sub = f'<text x="{W//2}" y="38" text-anchor="middle" font-size="13" fill="#666">{subtitulo}</text>' if subtitulo else ""
    return f"""<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}">
<rect width="{W}" height="{H}" fill="white"/>
<text x="{W//2}" y="22" text-anchor="middle" font-size="15" font-weight="bold" fill="#222">{titulo}</text>
{sub}"""

def svg_eixos(x0, y0, x1, y1):
    return f"""<line x1="{x0}" y1="{y0}" x2="{x0}" y2="{y1}" stroke="#999" stroke-width="1"/>
<line x1="{x0}" y1="{y1}" x2="{x1}" y2="{y1}" stroke="#999" stroke-width="1"/>"""

def svg_grid_h(x0, x1, y_vals, cor="#eee"):
    linhas = ""
    for y in y_vals:
        linhas += f'<line x1="{x0}" y1="{y:.1f}" x2="{x1}" y2="{y:.1f}" stroke="{cor}" stroke-width="0.8"/>\n'
    return linhas

def svg_legenda(x, y, algoritmos, colunas=1):
    svgs = []
    col_w = 160
    for i, alg in enumerate(algoritmos):
        col = i % colunas
        row = i // colunas
        lx = x + col * col_w
        ly = y + row * 20
        cor = CORES.get(alg, "#888")
        svgs.append(
            f'<rect x="{lx}" y="{ly-7}" width="18" height="10" rx="2" fill="{cor}" opacity="0.85"/>'
            f'<text x="{lx+22}" y="{ly+1}" font-size="12" fill="#333">{alg}</text>'
        )
    return "\n".join(svgs)

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 1 — Tempo vs N por algoritmo (uma linha por algo, uma figura por
#             tipo_entrada × estrutura)
# ══════════════════════════════════════════════════════════════════════════════
def grafico_tempo_vs_n(dados):
    print("\n[Gráfico 1] Tempo vs N — por algoritmo")

    estruturas  = ["Vetor", "ListaDinamica", "ListaEstatica"]
    entradas    = ["Aleatorio", "Ordenado", "InversamenteOrdenado"]
    algoritmos  = ["BubbleSort","InsertionSort","SelectionSort","QuickSort","MergeSort"]
    volumes     = [100, 1000, 10000]

    for estrutura in estruturas:
        for entrada in entradas:
            W, H = 680, 400
            ML, MR, MT, MB = 75, 20, 55, 60
            x0 = ML; x1 = W - MR; y0 = MT; y1 = H - MB

            # Coleta tempos por algoritmo
            tempos = {}
            for alg in algoritmos:
                pts = []
                for row in dados:
                    if (row["algoritmo"] == alg and
                        row["estrutura"] == estrutura and
                        row["tipo_entrada"] == entrada):
                        pts.append((row["n"], row["media_tempo_ms"]))
                pts.sort()
                if pts:
                    tempos[alg] = pts

            if not tempos:
                continue

            # Escala X (log pois N varia 100x)
            x_pos = {100: x0 + 80, 1000: x0 + (x1-x0)//2, 10000: x1 - 30}

            # Escala Y
            all_vals = [v for pts in tempos.values() for _, v in pts]
            scale_y, vmin, vmax = linear_scale(all_vals, y0, y1)

            # Ticks Y
            n_ticks = 5
            tick_step = (vmax - vmin) / n_ticks
            ticks_y = [vmin + i * tick_step for i in range(n_ticks + 1)]

            titulo    = f"Tempo de Execução × Volume de Dados"
            subtitulo = f"{estrutura}  |  {NOMES_ENTRADA[entrada]}"
            svg = svg_header(W, H, titulo, subtitulo)

            # Grid
            grid_ys = [scale_y(t) for t in ticks_y]
            svg += svg_grid_h(x0, x1, grid_ys)

            # Eixos
            svg += svg_eixos(x0, y0, x1, y1)

            # Ticks e rótulos eixo X
            for n, xp in x_pos.items():
                svg += f'<line x1="{xp}" y1="{y1}" x2="{xp}" y2="{y1+5}" stroke="#999" stroke-width="1"/>'
                svg += f'<text x="{xp}" y="{y1+18}" text-anchor="middle" font-size="11" fill="#555">N={n}</text>'

            # Ticks e rótulos eixo Y
            for tv, yp in zip(ticks_y, grid_ys):
                svg += f'<line x1="{x0-4}" y1="{yp:.1f}" x2="{x0}" y2="{yp:.1f}" stroke="#999" stroke-width="1"/>'
                svg += f'<text x="{x0-8}" y="{yp+4:.1f}" text-anchor="end" font-size="10" fill="#555">{tv:.3f}</text>'

            # Rótulo eixo Y
            svg += f'<text transform="rotate(-90)" x="{-(y0+y1)//2}" y="14" text-anchor="middle" font-size="12" fill="#555">Tempo médio (ms)</text>'

            # Linhas dos algoritmos
            for alg, pts in tempos.items():
                cor = CORES[alg]
                coords = [(x_pos[n], scale_y(v)) for n, v in pts if n in x_pos]
                if len(coords) < 2:
                    continue
                path = "M " + " L ".join(f"{x:.1f},{y:.1f}" for x, y in coords)
                svg += f'<path d="{path}" fill="none" stroke="{cor}" stroke-width="2.2" opacity="0.9"/>'
                for px, py in coords:
                    svg += f'<circle cx="{px:.1f}" cy="{py:.1f}" r="4" fill="{cor}" stroke="white" stroke-width="1.2"/>'

            # Legenda
            leg_x = x0 + 10
            leg_y = y0 + 15
            svg += svg_legenda(leg_x, leg_y, list(tempos.keys()), colunas=3)

            svg += "</svg>"
            nome = f"g1_tempo_vs_n_{estrutura}_{entrada}.svg"
            salvar_svg(svg, nome)

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 2 — Comparação de estruturas por algoritmo (fixo N=10000)
# ══════════════════════════════════════════════════════════════════════════════
def grafico_estruturas(dados):
    print("\n[Gráfico 2] Impacto da estrutura — N=10000")

    algoritmos  = ["BubbleSort","InsertionSort","SelectionSort","QuickSort","MergeSort"]
    estruturas  = ["Vetor", "ListaDinamica", "ListaEstatica"]
    entradas    = ["Aleatorio", "Ordenado", "InversamenteOrdenado"]
    cores_est   = {"Vetor": "#378ADD", "ListaDinamica": "#1D9E75", "ListaEstatica": "#E24B4A"}
    N           = 10000

    for entrada in entradas:
        W, H = 700, 420
        ML, MR, MT, MB = 80, 20, 55, 90
        x0 = ML; x1 = W - MR; y0 = MT; y1 = H - MB

        n_alg   = len(algoritmos)
        n_est   = len(estruturas)
        bar_w   = 28
        group_w = n_est * bar_w + 20
        total_w = n_alg * group_w

        # Valores
        valores = defaultdict(dict)
        for row in dados:
            if row["n"] == N and row["tipo_entrada"] == entrada:
                valores[row["algoritmo"]][row["estrutura"]] = row["media_tempo_ms"]

        all_vals = [v for d in valores.values() for v in d.values()]
        if not all_vals:
            continue

        scale_y, vmin, vmax = linear_scale(all_vals, y0, y1)
        n_ticks = 5
        tick_step = (vmax - vmin) / n_ticks
        ticks_y = [vmin + i * tick_step for i in range(n_ticks + 1)]
        grid_ys = [scale_y(t) for t in ticks_y]

        titulo    = f"Impacto da Estrutura de Dados no Desempenho"
        subtitulo = f"N = {N}  |  {NOMES_ENTRADA[entrada]}"
        svg = svg_header(W, H, titulo, subtitulo)
        svg += svg_grid_h(x0, x1, grid_ys)
        svg += svg_eixos(x0, y0, x1, y1)

        # Ticks Y
        for tv, yp in zip(ticks_y, grid_ys):
            svg += f'<line x1="{x0-4}" y1="{yp:.1f}" x2="{x0}" y2="{yp:.1f}" stroke="#999" stroke-width="1"/>'
            svg += f'<text x="{x0-8}" y="{yp+4:.1f}" text-anchor="end" font-size="10" fill="#555">{tv:.3f}</text>'

        svg += f'<text transform="rotate(-90)" x="{-(y0+y1)//2}" y="14" text-anchor="middle" font-size="12" fill="#555">Tempo médio (ms)</text>'

        # Barras
        avail_w = x1 - x0
        spacing = avail_w / n_alg
        for gi, alg in enumerate(algoritmos):
            gx = x0 + gi * spacing + spacing / 2 - (n_est * bar_w) / 2
            for ei, est in enumerate(estruturas):
                v = valores[alg].get(est, 0)
                bx = gx + ei * bar_w
                by = scale_y(v)
                bh = y1 - by
                cor = cores_est[est]
                svg += f'<rect x="{bx:.1f}" y="{by:.1f}" width="{bar_w-4}" height="{bh:.1f}" fill="{cor}" opacity="0.82" rx="2"/>'

            # Rótulo do algoritmo
            cx = x0 + gi * spacing + spacing / 2
            svg += f'<text x="{cx:.1f}" y="{y1+14}" text-anchor="middle" font-size="10" fill="#444">{alg}</text>'

        # Legenda estruturas
        leg_items = list(cores_est.items())
        leg_x = x0 + 10
        leg_y = H - 22
        for i, (est, cor) in enumerate(leg_items):
            lx = leg_x + i * 180
            svg += f'<rect x="{lx}" y="{leg_y-8}" width="16" height="10" rx="2" fill="{cor}" opacity="0.82"/>'
            svg += f'<text x="{lx+20}" y="{leg_y+1}" font-size="12" fill="#333">{est}</text>'

        svg += "</svg>"
        nome = f"g2_estruturas_N{N}_{entrada}.svg"
        salvar_svg(svg, nome)

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 3 — Melhor / Médio / Pior caso por algoritmo (barras agrupadas)
#             Estrutura = Vetor, N = 10000
# ══════════════════════════════════════════════════════════════════════════════
def grafico_casos(dados):
    print("\n[Gráfico 3] Melhor / Médio / Pior caso — Vetor, N=10000")

    algoritmos = ["BubbleSort","InsertionSort","SelectionSort","QuickSort","MergeSort"]
    mapa_caso  = {
        "Ordenado":             ("Melhor caso", "#1D9E75"),
        "Aleatorio":            ("Caso médio",  "#378ADD"),
        "InversamenteOrdenado": ("Pior caso",   "#E24B4A"),
    }
    N = 10000; est = "Vetor"

    W, H = 700, 420
    ML, MR, MT, MB = 80, 20, 55, 80
    x0 = ML; x1 = W - MR; y0 = MT; y1 = H - MB

    valores = defaultdict(dict)
    for row in dados:
        if row["n"] == N and row["estrutura"] == est:
            valores[row["algoritmo"]][row["tipo_entrada"]] = row["media_tempo_ms"]

    all_vals = [v for d in valores.values() for v in d.values()]
    scale_y, vmin, vmax = linear_scale(all_vals, y0, y1)
    n_ticks = 5
    ticks_y = [vmin + i*(vmax-vmin)/n_ticks for i in range(n_ticks+1)]
    grid_ys = [scale_y(t) for t in ticks_y]

    svg = svg_header(W, H, "Melhor / Caso Médio / Pior Caso por Algoritmo",
                     f"Estrutura: {est}  |  N = {N}")
    svg += svg_grid_h(x0, x1, grid_ys)
    svg += svg_eixos(x0, y0, x1, y1)

    for tv, yp in zip(ticks_y, grid_ys):
        svg += f'<line x1="{x0-4}" y1="{yp:.1f}" x2="{x0}" y2="{yp:.1f}" stroke="#999" stroke-width="1"/>'
        svg += f'<text x="{x0-8}" y="{yp+4:.1f}" text-anchor="end" font-size="10" fill="#555">{tv:.3f}</text>'
    svg += f'<text transform="rotate(-90)" x="{-(y0+y1)//2}" y="14" text-anchor="middle" font-size="12" fill="#555">Tempo médio (ms)</text>'

    n_alg   = len(algoritmos)
    n_casos = 3
    bar_w   = 28
    spacing = (x1 - x0) / n_alg

    entradas_ordem = ["Ordenado", "Aleatorio", "InversamenteOrdenado"]
    for gi, alg in enumerate(algoritmos):
        gx = x0 + gi * spacing + spacing/2 - (n_casos * bar_w)/2
        for ei, entrada in enumerate(entradas_ordem):
            v   = valores[alg].get(entrada, 0)
            label, cor = mapa_caso[entrada]
            bx  = gx + ei * bar_w
            by  = scale_y(v)
            bh  = y1 - by
            svg += f'<rect x="{bx:.1f}" y="{by:.1f}" width="{bar_w-4}" height="{bh:.1f}" fill="{cor}" opacity="0.82" rx="2"/>'
        cx = x0 + gi * spacing + spacing/2
        svg += f'<text x="{cx:.1f}" y="{y1+14}" text-anchor="middle" font-size="10" fill="#444">{alg}</text>'

    # Legenda
    leg_x = x0 + 10
    leg_y = H - 18
    for i, entrada in enumerate(entradas_ordem):
        label, cor = mapa_caso[entrada]
        lx = leg_x + i * 190
        svg += f'<rect x="{lx}" y="{leg_y-8}" width="16" height="10" rx="2" fill="{cor}" opacity="0.82"/>'
        svg += f'<text x="{lx+20}" y="{leg_y+1}" font-size="12" fill="#333">{label}</text>'

    svg += "</svg>"
    salvar_svg(svg, "g3_melhor_medio_pior.svg")

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 4 — Número de comparações vs N (linha, Vetor, caso médio)
# ══════════════════════════════════════════════════════════════════════════════
def grafico_comparacoes(dados):
    print("\n[Gráfico 4] Número de comparações vs N")

    algoritmos = ["BubbleSort","InsertionSort","SelectionSort","QuickSort","MergeSort"]
    volumes    = [100, 1000, 10000]
    est        = "Vetor"
    entrada    = "Aleatorio"

    W, H = 680, 400
    ML, MR, MT, MB = 80, 20, 55, 60
    x0 = ML; x1 = W - MR; y0 = MT; y1 = H - MB

    x_pos = {100: x0+80, 1000: x0+(x1-x0)//2, 10000: x1-30}

    comps = {}
    for alg in algoritmos:
        pts = []
        for row in dados:
            if row["algoritmo"]==alg and row["estrutura"]==est and row["tipo_entrada"]==entrada:
                pts.append((row["n"], row["media_comparacoes"]))
        pts.sort()
        if pts:
            comps[alg] = pts

    all_vals = [v for pts in comps.values() for _, v in pts]
    # Usa escala log pois as diferenças são grandes
    scale_y, lmin, lmax = log_scale(all_vals, y0, y1)

    # Ticks Y em escala log
    import math
    ticks_raw = [10**e for e in range(int(lmin), int(lmax)+2) if int(lmin) <= e <= int(lmax)+1]
    ticks_raw = [t for t in ticks_raw if min(all_vals) <= t <= max(all_vals)*1.5]

    svg = svg_header(W, H, "Número de Comparações × Volume de Dados",
                     f"Vetor  |  Entrada aleatória  |  Escala logarítmica")

    # Grid
    grid_ys = [scale_y(t) for t in ticks_raw]
    svg += svg_grid_h(x0, x1, grid_ys)
    svg += svg_eixos(x0, y0, x1, y1)

    # Ticks Y
    for tv, yp in zip(ticks_raw, grid_ys):
        if y0 <= yp <= y1:
            svg += f'<line x1="{x0-4}" y1="{yp:.1f}" x2="{x0}" y2="{yp:.1f}" stroke="#999" stroke-width="1"/>'
            lbl = f"{int(tv):,}".replace(",", ".")
            svg += f'<text x="{x0-8}" y="{yp+4:.1f}" text-anchor="end" font-size="9" fill="#555">{lbl}</text>'

    # Ticks X
    for n, xp in x_pos.items():
        svg += f'<line x1="{xp}" y1="{y1}" x2="{xp}" y2="{y1+5}" stroke="#999" stroke-width="1"/>'
        svg += f'<text x="{xp}" y="{y1+18}" text-anchor="middle" font-size="11" fill="#555">N={n}</text>'

    svg += f'<text transform="rotate(-90)" x="{-(y0+y1)//2}" y="14" text-anchor="middle" font-size="12" fill="#555">Comparações (log)</text>'

    # Linhas
    for alg, pts in comps.items():
        cor = CORES[alg]
        coords = [(x_pos[n], scale_y(v)) for n, v in pts if n in x_pos]
        if len(coords) < 2:
            continue
        path = "M " + " L ".join(f"{x:.1f},{y:.1f}" for x, y in coords)
        svg += f'<path d="{path}" fill="none" stroke="{cor}" stroke-width="2.2" opacity="0.9"/>'
        for px, py in coords:
            svg += f'<circle cx="{px:.1f}" cy="{py:.1f}" r="4" fill="{cor}" stroke="white" stroke-width="1.2"/>'

    svg += svg_legenda(x0+10, y0+15, list(comps.keys()), colunas=3)
    svg += "</svg>"
    salvar_svg(svg, "g4_comparacoes_vs_n.svg")

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 5 — Algoritmo ideal: QuickSort vs MergeSort em todos os cenários
# ══════════════════════════════════════════════════════════════════════════════
def grafico_algoritmo_ideal(dados):
    print("\n[Gráfico 5] Algoritmo ideal — QuickSort vs MergeSort")

    candidatos = ["QuickSort", "MergeSort"]
    entradas   = ["Aleatorio", "Ordenado", "InversamenteOrdenado"]
    estruturas = ["Vetor", "ListaDinamica", "ListaEstatica"]
    volumes    = [100, 1000, 10000]

    cores_cand = {"QuickSort": "#378ADD", "MergeSort": "#7F77DD"}
    est_dash   = {"Vetor": "none", "ListaDinamica": "6,3", "ListaEstatica": "2,4"}
    est_marker = {"Vetor": 5, "ListaDinamica": 4, "ListaEstatica": 3}

    for entrada in entradas:
        W, H = 700, 430
        ML, MR, MT, MB = 80, 20, 55, 90
        x0 = ML; x1 = W - MR; y0 = MT; y1 = H - MB

        x_pos = {100: x0+80, 1000: x0+(x1-x0)//2, 10000: x1-30}

        series = {}
        for alg in candidatos:
            for est in estruturas:
                pts = []
                for row in dados:
                    if (row["algoritmo"]==alg and row["estrutura"]==est
                            and row["tipo_entrada"]==entrada):
                        pts.append((row["n"], row["media_tempo_ms"]))
                pts.sort()
                if pts:
                    series[(alg, est)] = pts

        all_vals = [v for pts in series.values() for _, v in pts]
        if not all_vals:
            continue
        scale_y, vmin, vmax = linear_scale(all_vals, y0, y1)
        n_ticks = 5
        ticks_y = [vmin + i*(vmax-vmin)/n_ticks for i in range(n_ticks+1)]
        grid_ys = [scale_y(t) for t in ticks_y]

        svg = svg_header(W, H, "QuickSort vs MergeSort — Algoritmo Ideal",
                         f"{NOMES_ENTRADA[entrada]}")
        svg += svg_grid_h(x0, x1, grid_ys)
        svg += svg_eixos(x0, y0, x1, y1)

        for tv, yp in zip(ticks_y, grid_ys):
            svg += f'<line x1="{x0-4}" y1="{yp:.1f}" x2="{x0}" y2="{yp:.1f}" stroke="#999" stroke-width="1"/>'
            svg += f'<text x="{x0-8}" y="{yp+4:.1f}" text-anchor="end" font-size="10" fill="#555">{tv:.4f}</text>'
        for n, xp in x_pos.items():
            svg += f'<line x1="{xp}" y1="{y1}" x2="{xp}" y2="{y1+5}" stroke="#999" stroke-width="1"/>'
            svg += f'<text x="{xp}" y="{y1+18}" text-anchor="middle" font-size="11" fill="#555">N={n}</text>'
        svg += f'<text transform="rotate(-90)" x="{-(y0+y1)//2}" y="14" text-anchor="middle" font-size="12" fill="#555">Tempo médio (ms)</text>'

        for (alg, est), pts in series.items():
            cor   = cores_cand[alg]
            dash  = est_dash[est]
            r     = est_marker[est]
            coords = [(x_pos[n], scale_y(v)) for n, v in pts if n in x_pos]
            if len(coords) < 2:
                continue
            path = "M " + " L ".join(f"{x:.1f},{y:.1f}" for x, y in coords)
            svg += f'<path d="{path}" fill="none" stroke="{cor}" stroke-width="2" stroke-dasharray="{dash}" opacity="0.9"/>'
            for px, py in coords:
                svg += f'<circle cx="{px:.1f}" cy="{py:.1f}" r="{r}" fill="{cor}" stroke="white" stroke-width="1"/>'

        # Legenda dupla: algoritmos + estruturas
        leg_y = H - 56
        for i, alg in enumerate(candidatos):
            lx = x0 + i * 200
            cor = cores_cand[alg]
            svg += f'<rect x="{lx}" y="{leg_y-8}" width="28" height="4" rx="1" fill="{cor}"/>'
            svg += f'<text x="{lx+34}" y="{leg_y+1}" font-size="12" fill="#333">{alg}</text>'

        leg_y2 = H - 28
        for i, (est, dash) in enumerate(est_dash.items()):
            lx = x0 + i * 200
            d  = f'stroke-dasharray="{dash}"' if dash != "none" else ""
            svg += f'<line x1="{lx}" y1="{leg_y2-4}" x2="{lx+28}" y2="{leg_y2-4}" stroke="#666" stroke-width="2" {d}/>'
            svg += f'<text x="{lx+34}" y="{leg_y2+1}" font-size="12" fill="#333">{est}</text>'

        svg += "</svg>"
        nome = f"g5_ideal_{entrada}.svg"
        salvar_svg(svg, nome)

# ══════════════════════════════════════════════════════════════════════════════
# GRÁFICO 6 — Tabela resumo: todos algoritmos, N=10000, entrada aleatória
# ══════════════════════════════════════════════════════════════════════════════
def grafico_tabela_resumo(dados):
    print("\n[Gráfico 6] Tabela resumo")

    algoritmos = ["BubbleSort","InsertionSort","SelectionSort","QuickSort","MergeSort"]
    entradas   = ["Ordenado","Aleatorio","InversamenteOrdenado"]
    labels_ent = ["Melhor caso\n(Ordenado)", "Caso médio\n(Aleatório)", "Pior caso\n(Inv. Ordenado)"]
    est        = "Vetor"
    N          = 10000

    # Coleta
    tabela = {}
    for row in dados:
        if row["n"]==N and row["estrutura"]==est:
            tabela[(row["algoritmo"], row["tipo_entrada"])] = row["media_tempo_ms"]

    W, H = 680, 300
    svg = svg_header(W, H, f"Resumo de Desempenho — {est}, N={N}",
                     "Tempo médio em ms")

    col_w  = 130
    row_h  = 36
    off_x  = 110
    off_y  = 65

    # Cabeçalho colunas
    for ci, lbl in enumerate(["Melhor caso", "Caso médio", "Pior caso"]):
        cx = off_x + ci * col_w + col_w // 2
        svg += f'<rect x="{off_x + ci*col_w}" y="{off_y-row_h}" width="{col_w}" height="{row_h}" fill="#f0f0f0" stroke="#ccc" stroke-width="0.5"/>'
        svg += f'<text x="{cx}" y="{off_y-row_h//2+5}" text-anchor="middle" font-size="11" font-weight="bold" fill="#333">{lbl}</text>'

    # Linhas
    for ri, alg in enumerate(algoritmos):
        ry = off_y + ri * row_h
        bg = "#fafafa" if ri % 2 == 0 else "#ffffff"
        # Rótulo algoritmo
        svg += f'<rect x="0" y="{ry}" width="{off_x}" height="{row_h}" fill="{bg}" stroke="#ccc" stroke-width="0.5"/>'
        cor = CORES[alg]
        svg += f'<rect x="4" y="{ry+10}" width="8" height="8" rx="2" fill="{cor}"/>'
        svg += f'<text x="18" y="{ry+row_h//2+4}" font-size="11" fill="#222">{alg}</text>'

        # Células
        vals = [tabela.get((alg, e), 0) for e in entradas]
        vmin_row = min(vals)
        for ci, (entrada, v) in enumerate(zip(entradas, vals)):
            cx  = off_x + ci * col_w
            mid = cx + col_w // 2
            # Destaca o menor valor da linha
            cell_bg = "#e8f5e9" if v == vmin_row else bg
            svg += f'<rect x="{cx}" y="{ry}" width="{col_w}" height="{row_h}" fill="{cell_bg}" stroke="#ccc" stroke-width="0.5"/>'
            svg += f'<text x="{mid}" y="{ry+row_h//2+4}" text-anchor="middle" font-size="11" fill="#222">{v:.4f} ms</text>'

    svg += "</svg>"
    salvar_svg(svg, "g6_tabela_resumo.svg")

# ══════════════════════════════════════════════════════════════════════════════
# MAIN
# ══════════════════════════════════════════════════════════════════════════════
if __name__ == "__main__":
    print("Lendo resultados.csv...")
    dados = ler_csv("data/resultados.csv")
    print(f"  {len(dados)} linhas carregadas.")

    grafico_tempo_vs_n(dados)
    grafico_estruturas(dados)
    grafico_casos(dados)
    grafico_comparacoes(dados)
    grafico_algoritmo_ideal(dados)
    grafico_tabela_resumo(dados)

    print(f"\nTodos os gráficos salvos em ./{OUTPUT_DIR}/")