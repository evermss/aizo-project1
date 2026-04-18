import os
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter


RESEARCH_DIR = os.path.join("results", "research")
REPORT_DIR = os.path.join("results", "report")
CHARTS_DIR = os.path.join(REPORT_DIR, "charts")
TABLES_DIR = os.path.join(REPORT_DIR, "tables")


def ensure_dirs():
    os.makedirs(CHARTS_DIR, exist_ok=True)
    os.makedirs(TABLES_DIR, exist_ok=True)


def load_csv(filename):
    path = os.path.join(RESEARCH_DIR, filename)
    if not os.path.exists(path):
        print(f"Brak pliku: {path}")
        return None
    return pd.read_csv(path)


def save_table(df, filename):
    df.to_csv(os.path.join(TABLES_DIR, filename), index=False)


def sci_formatter():
    def _fmt(x, pos):
        if x == 0:
            return "0"
        exponent = int(f"{x:.0e}".split("e")[1])
        mantissa = x / (10 ** exponent)
        mantissa_str = f"{mantissa:.2f}".rstrip("0").rstrip(".")
        return rf"${mantissa_str}\times 10^{{{exponent}}}$"
    return FuncFormatter(_fmt)


def apply_axis_style(ax, log_scale=False):
    if log_scale:
        ax.set_yscale("log")
    ax.yaxis.set_major_formatter(sci_formatter())


def save_bar_chart(x, y, xlabel, ylabel, title, output_name, rotation=0, log_scale=False):
    fig, ax = plt.subplots(figsize=(9, 5))
    ax.bar(x, y)
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    ax.tick_params(axis="x", rotation=rotation)
    apply_axis_style(ax, log_scale=log_scale)
    fig.tight_layout()
    fig.savefig(os.path.join(CHARTS_DIR, output_name), dpi=200)
    plt.close(fig)


def save_line_chart(groups, xlabel, ylabel, title, output_name, log_scale=False):
    fig, ax = plt.subplots(figsize=(10, 6))
    for label, group in groups:
        ax.plot(group["x"], group["y"], marker="o", label=label)

    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    ax.legend()
    apply_axis_style(ax, log_scale=log_scale)
    fig.tight_layout()
    fig.savefig(os.path.join(CHARTS_DIR, output_name), dpi=200)
    plt.close(fig)


def normalize_distribution_name(value):
    mapping = {
        "half_sorted": "ascending50Per",
        "ascending50Per": "ascending50Per",
        "random": "random",
        "ascending": "ascending",
        "descending": "descending",
    }
    return mapping.get(str(value), str(value))


def chart_alpha():
    df = load_csv("alpha_results.csv")
    if df is None or df.empty:
        return

    df = df.copy()

    quick_df = df[df["algorithm"] == "quick"].copy()
    if not quick_df.empty:
        quick_df = quick_df.sort_values("average_us")
        save_bar_chart(
            quick_df["variant"],
            quick_df["average_us"],
            "Wariant pivota",
            "Średni czas [µs]",
            "Badanie α - Quicksort",
            "alpha_quick.png",
            log_scale=True
        )
        save_table(
            quick_df[["algorithm", "structure", "variant", "size", "average_us", "min_us", "max_us"]],
            "alpha_quick_summary.csv"
        )

    shell_df = df[df["algorithm"] == "shell"].copy()
    if not shell_df.empty:
        shell_df = shell_df.sort_values("average_us")
        save_bar_chart(
            shell_df["variant"],
            shell_df["average_us"],
            "Wariant odstępów",
            "Średni czas [µs]",
            "Badanie α - Shellsort",
            "alpha_shell.png",
            log_scale=True
        )
        save_table(
            shell_df[["algorithm", "structure", "variant", "size", "average_us", "min_us", "max_us"]],
            "alpha_shell_summary.csv"
        )

    if not quick_df.empty:
        for structure_name, group in quick_df.groupby("structure"):
            group = group.sort_values("average_us")
            save_bar_chart(
                group["variant"],
                group["average_us"],
                "Wariant pivota",
                "Średni czas [µs]",
                f"Badanie α - Quicksort - {structure_name}",
                f"alpha_quick_{structure_name}.png",
                log_scale=True
            )

    if not shell_df.empty:
        for structure_name, group in shell_df.groupby("structure"):
            group = group.sort_values("average_us")
            save_bar_chart(
                group["variant"],
                group["average_us"],
                "Wariant odstępów",
                "Średni czas [µs]",
                f"Badanie α - Shellsort - {structure_name}",
                f"alpha_shell_{structure_name}.png",
                log_scale=True
            )


def chart_A():
    df = load_csv("A_results.csv")
    if df is None or df.empty:
        return

    df = df.copy()
    df["series"] = df["algorithm"] + " | " + df["structure"]

    groups = []
    for series_name, group in df.groupby("series"):
        group = group.sort_values("size")
        groups.append((
            series_name,
            pd.DataFrame({
                "x": group["size"],
                "y": group["average_us"]
            })
        ))

    save_line_chart(
        groups,
        "Rozmiar danych",
        "Średni czas [µs]",
        "Badanie A - wpływ liczebności zbioru",
        "A_chart.png",
        log_scale=False
    )

    structures = ["array", "single_list", "double_list"]

    for structure_name in structures:
        struct_df = df[df["structure"] == structure_name].copy()
        if struct_df.empty:
            continue

        groups = []
        for algorithm_name, group in struct_df.groupby("algorithm"):
            group = group.sort_values("size")
            groups.append((
                algorithm_name,
                pd.DataFrame({
                    "x": group["size"],
                    "y": group["average_us"]
                })
            ))

        save_line_chart(
            groups,
            "Rozmiar danych",
            "Średni czas [µs]",
            f"Badanie A - {structure_name}",
            f"A_{structure_name}.png",
            log_scale=False
        )

    summary = (
        df.groupby(["algorithm", "structure"], as_index=False)["average_us"]
        .mean()
        .sort_values("average_us")
    )
    save_table(summary, "A_summary.csv")


def chart_B():
    df = load_csv("B_results.csv")
    if df is None or df.empty:
        return

    df = df.copy()
    df["variant"] = df["variant"].apply(normalize_distribution_name)

    order = ["random", "descending", "ascending", "ascending50Per"]

    fig, ax = plt.subplots(figsize=(10, 6))
    for structure_name, group in df.groupby("structure"):
        group = group.copy()
        group["variant"] = pd.Categorical(group["variant"], categories=order, ordered=True)
        group = group.sort_values("variant")
        ax.plot(group["variant"].astype(str), group["average_us"], marker="o", label=structure_name)

    ax.set_xlabel("Rozkład danych")
    ax.set_ylabel("Średni czas [µs]")
    ax.set_title("Badanie B - wpływ rozkładu danych")
    ax.legend()
    apply_axis_style(ax, log_scale=True)
    fig.tight_layout()
    fig.savefig(os.path.join(CHARTS_DIR, "B_chart.png"), dpi=200)
    plt.close(fig)

    structures = ["array", "single_list", "double_list"]

    for structure_name in structures:
        struct_df = df[df["structure"] == structure_name].copy()
        if struct_df.empty:
            continue

        struct_df["variant"] = pd.Categorical(
            struct_df["variant"],
            categories=order,
            ordered=True
        )
        struct_df = struct_df.sort_values("variant")

        save_bar_chart(
            struct_df["variant"].astype(str),
            struct_df["average_us"],
            "Rozkład danych",
            "Średni czas [µs]",
            f"Badanie B - {structure_name}",
            f"B_{structure_name}.png",
            log_scale=True
        )

    save_table(
        df[["algorithm", "structure", "variant", "size", "average_us", "min_us", "max_us"]]
        .sort_values(["structure", "average_us"]),
        "B_summary.csv"
    )


def chart_C():
    df = load_csv("C_results.csv")
    if df is None or df.empty:
        return

    df = df.copy()

    def extract_type(structure_name):
        if structure_name.startswith("template_array_"):
            return structure_name.replace("template_array_", "")
        return structure_name

    df["data_type_name"] = df["structure"].apply(extract_type)

    save_bar_chart(
        df["data_type_name"],
        df["average_us"],
        "Typ danych",
        "Średni czas [µs]",
        "Badanie C - wpływ typu danych",
        "C_chart.png",
        rotation=20,
        log_scale=True
    )

    save_table(
        df[["data_type_name", "size", "average_us", "min_us", "max_us"]]
        .sort_values("average_us"),
        "C_summary.csv"
    )


def chart_omega():
    df = load_csv("omega_results.csv")
    if df is None or df.empty:
        return

    save_bar_chart(
        df["structure"],
        df["average_us"],
        "Struktura",
        "Średni czas [µs]",
        "Badanie Ω - porównanie struktur",
        "omega_chart.png",
        rotation=20,
        log_scale=True
    )

    save_table(
        df[["algorithm", "structure", "size", "average_us", "min_us", "max_us"]]
        .sort_values("average_us"),
        "omega_summary.csv"
    )


def main():
    ensure_dirs()
    chart_alpha()
    chart_A()
    chart_B()
    chart_C()
    chart_omega()
    print("Gotowe. Wykresy zapisano w results/report/charts, a tabele w results/report/tables.")


if __name__ == "__main__":
    main()