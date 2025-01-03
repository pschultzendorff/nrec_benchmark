import pathlib
from datetime import datetime
from typing import Any

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def read_results() -> pd.DataFrame:
    files = pathlib.Path(__file__).parent.glob("results_*.txt")
    data: list[dict[str, Any]] = []
    for file in files:
        time = datetime.strptime("_".join(file.stem.split("_")[1:]), "%Y-%m-%d_%H-%M-%S")
        with open(file, "r") as f:
            for line in f.readlines()[1:]:
                parts: list[str] = line.strip().split(",")
                data.append({
                    "timestamp": time.timestamp(),
                    "benchmark": parts[0],
                    "minimum time": float(parts[1]),
                    "maximum time": float(parts[2]),
                    "average time": float(parts[3]),
                    "standard deviation": float(parts[4])
                })
    return pd.DataFrame(data)

def plot_performance() -> None:
    df = read_results()
    # df["timestamp"] = pd.to_datetime(df["timestamp"])

    sns.set(style="whitegrid")

    fig, axes = plt.subplots(3, 1, figsize=(10, 6))

    for benchmark, ax in zip(df["benchmark"].unique(), axes):
        sns.lineplot(x="timestamp", y="minimum time", data=df[df["benchmark"] == benchmark], ax=ax)
        sns.lineplot(x="timestamp", y="maximum time", data=df[df["benchmark"] == benchmark], ax=ax)
        sns.lineplot(x="timestamp", y="average time", data=df[df["benchmark"] == benchmark], ax=ax)
        ax.set_title(f"{benchmark} - Performance Over Time")
        ax.set_xlabel("Timestamp (s)")
        ax.set_ylabel("Runtime (ms)")
        fig.tight_layout()
    plt.savefig("performance.png")
    plt.show()

plot_performance()
