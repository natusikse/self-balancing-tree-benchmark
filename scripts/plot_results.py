from __future__ import annotations

import csv
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


PROJECT_ROOT = Path(__file__).resolve().parent.parent
RESULTS_FILE = PROJECT_ROOT / "results" / "benchmark_results.csv"
OUTPUT_FILE = PROJECT_ROOT / "results" / "average_depth.svg"

TREE_ORDER = ["BST", "AVL", "Splay"]
WORKLOAD_ORDER = [
    "Ordered / uniform",
    "Random / uniform",
    "Random / hot-key",
]

COLORS = {
    "BST": "#4C78A8",
    "AVL": "#F58518",
    "Splay": "#54A24B",
}


def load_results() -> dict[str, dict[str, float]]:
    results: dict[str, dict[str, float]] = {}

    with RESULTS_FILE.open(newline="", encoding="utf-8") as file:
        reader = csv.DictReader(file)

        for row in reader:
            workload = row["workload"]
            tree = row["tree"]
            average_depth = float(row["average_depth"])

            results.setdefault(workload, {})[tree] = average_depth

    return results


def add_value_labels(axis: plt.Axes, containers: list) -> None:
    for container in containers:
        axis.bar_label(
            container,
            fmt="%.2f",
            padding=3,
            fontsize=8,
        )


def draw_grouped_bars(
    axis: plt.Axes,
    results: dict[str, dict[str, float]],
    workloads: list[str],
    logarithmic: bool,
) -> None:
    x_positions = np.arange(len(workloads))
    bar_width = 0.24
    containers = []

    for index, tree in enumerate(TREE_ORDER):
        offset = (index - 1) * bar_width
        values = [
            results[workload][tree]
            for workload in workloads
        ]

        container = axis.bar(
            x_positions + offset,
            values,
            bar_width,
            label=tree,
            color=COLORS[tree],
        )

        containers.append(container)

    axis.set_xticks(x_positions)
    axis.set_xticklabels(workloads)
    axis.set_ylabel("Average search depth")
    axis.grid(axis="y", linestyle="--", alpha=0.3)
    axis.set_axisbelow(True)

    if logarithmic:
        axis.set_yscale("log")
        axis.set_title("All workloads — logarithmic scale")
    else:
        axis.set_ylim(0, 13)
        axis.set_title("Random insertion workloads")
        add_value_labels(axis, containers)


def remove_trailing_whitespace(path: Path) -> None:
    svg_content = path.read_text(encoding="utf-8")

    cleaned_svg = "\n".join(
        line.rstrip()
        for line in svg_content.splitlines()
    )

    path.write_text(
        cleaned_svg + "\n",
        encoding="utf-8",
    )


def main() -> None:
    results = load_results()

    figure, axes = plt.subplots(
        1,
        2,
        figsize=(14, 6),
        constrained_layout=True,
    )

    draw_grouped_bars(
        axes[0],
        results,
        WORKLOAD_ORDER,
        logarithmic=True,
    )

    draw_grouped_bars(
        axes[1],
        results,
        [
            "Random / uniform",
            "Random / hot-key",
        ],
        logarithmic=False,
    )

    handles, labels = axes[0].get_legend_handles_labels()

    figure.legend(
        handles,
        labels,
        loc="upper center",
        ncol=3,
        frameon=False,
        bbox_to_anchor=(0.5, 1.03),
    )

    figure.suptitle(
        "BST, AVL, and Splay Tree Search Depth",
        fontsize=16,
        fontweight="bold",
    )

    figure.savefig(
        OUTPUT_FILE,
        format="svg",
        bbox_inches="tight",
    )

    plt.close(figure)

    remove_trailing_whitespace(OUTPUT_FILE)

    print(f"Chart written to {OUTPUT_FILE}")


if __name__ == "__main__":
    main()