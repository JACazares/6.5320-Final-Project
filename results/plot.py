import pandas as pd

import matplotlib.pyplot as plt

test_type = "empty"
d = 4

prefix = f"{test_type}/benchmarks d={d}/"

def build_time_plot(
    algorithm1,
    algorithm2,
    algorithm3,
    algorithm1_name,
    algorithm2_name,
    algorithm3_name,
    algorithm1_color,
    algorithm2_color,
    algorithm3_color,
):
    # Plotting N vs. build time for each algorithm
    plt.figure(
        figsize=(13, 8)
    )  # Set the figure size to 8 inches wide and 6 inches tall
    plt.plot(
        algorithm1["N"],
        algorithm1["Sorting Time (us)"],
        color=algorithm1_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm1_color,
        markerfacecolor=algorithm1_color,
        linestyle=":",
        label=algorithm1_name + " Sorting Time (us)",
    )
    plt.plot(
        algorithm1["N"],
        algorithm1["Build Time (us)"],
        color=algorithm1_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm1_color,
        markerfacecolor=algorithm1_color,
        linestyle=":",
        label=algorithm1_name + " Build Time (us)",
    )
    plt.plot(
        algorithm1["N"],
        algorithm1["Total Time (us)"],
        color=algorithm1_color,
        marker="o",
        markersize=5,
        label=algorithm1_name + " Total Time (us)",
    )

    plt.plot(
        algorithm2["N"],
        algorithm2["Sorting Time (us)"],
        color=algorithm2_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm2_color,
        markerfacecolor=algorithm2_color,
        linestyle=":",
        label=algorithm2_name + " Sorting Time (us)",
    )
    plt.plot(
        algorithm2["N"],
        algorithm2["Build Time (us)"],
        color=algorithm2_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm2_color,
        markerfacecolor=algorithm2_color,
        linestyle=":",
        label=algorithm2_name + " Build Time (us)",
    )
    plt.plot(
        algorithm2["N"],
        algorithm2["Total Time (us)"],
        color=algorithm2_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm2_color,
        markerfacecolor=algorithm2_color,
        label=algorithm2_name + " Total Time (us)",
    )

    plt.plot(
        algorithm3["N"],
        algorithm3["Sorting Time (us)"],
        color=algorithm3_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm3_color,
        markerfacecolor=algorithm3_color,
        linestyle=":",
        label=algorithm3_name + " Sorting Time (us)",
    )
    plt.plot(
        algorithm3["N"],
        algorithm3["Build Time (us)"],
        color=algorithm3_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm3_color,
        markerfacecolor=algorithm3_color,
        linestyle=":",
        label=algorithm3_name + " Build Time (us)",
    )
    plt.plot(
        algorithm3["N"],
        algorithm3["Total Time (us)"],
        color=algorithm3_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm3_color,
        markerfacecolor=algorithm3_color,
        label=algorithm3_name + " Total Time (us)",
    )

    # Add labels to each line
    plt.annotate(
        algorithm1_name + " Total Time (us)",
        (algorithm1["N"].iloc[-1], algorithm1["Total Time (us)"].iloc[-1]),
        xytext=(5, -5),
        textcoords="offset points",
        color=algorithm1_color,
    )

    plt.annotate(
        algorithm2_name + " Sorting Time (us)",
        (algorithm2["N"].iloc[-1], algorithm2["Sorting Time (us)"].iloc[-1]),
        xytext=(5, -5),
        textcoords="offset points",
        color=algorithm2_color,
    )
    plt.annotate(
        algorithm2_name + " Build Time (us)",
        (algorithm2["N"].iloc[-1], algorithm2["Build Time (us)"].iloc[-1]),
        xytext=(5, -5),
        textcoords="offset points",
        color=algorithm2_color,
    )
    plt.annotate(
        algorithm2_name + " Total Time (us)",
        (algorithm2["N"].iloc[-1], algorithm2["Total Time (us)"].iloc[-1]),
        xytext=(5, -5),
        textcoords="offset points",
        color=algorithm2_color,
    )

    plt.annotate(
        algorithm3_name + " Sorting Time (us)",
        (algorithm3["N"].iloc[-1], algorithm3["Sorting Time (us)"].iloc[-1]),
        xytext=(5, -5),
        textcoords="offset points",
        color=algorithm3_color,
    )
    plt.annotate(
        algorithm3_name + " Build Time (us)",
        (algorithm3["N"].iloc[-1], algorithm3["Build Time (us)"].iloc[-1]),
        xytext=(5, -5),
        textcoords="offset points",
        color=algorithm3_color,
    )
    plt.annotate(
        algorithm3_name + " Total Time (us)",
        (algorithm3["N"].iloc[-1], algorithm3["Total Time (us)"].iloc[-1]),
        xytext=(5, -5),
        textcoords="offset points",
        color=algorithm3_color,
    )

    plt.xlabel("N")
    plt.ylabel("Time (us)")
    plt.title("N vs. Time")
    plt.legend()
    plt.show()


def query_time_plot(
    algorithm1_avg_query,
    algorithm1_std_query,
    algorithm2_avg_query,
    algorithm2_std_query,
    algorithm3_avg_query,
    algorithm3_std_query,
    algorithm1_name,
    algorithm2_name,
    algorithm3_name,
    algorithm1_color,
    algorithm2_color,
    algorithm3_color,
    query_answer_points,
):
    # Plot the average query time with error bars
    plt.figure(
        figsize=(13, 8)
    )  # Set the figure size to 8 inches wide and 6 inches tall
    plt.errorbar(
        algorithm1_avg_query.index,
        algorithm1_avg_query,
        yerr=algorithm1_std_query,
        color=algorithm1_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm1_color,
        markerfacecolor=algorithm1_color,
        label=algorithm1_name + " Query Time (us)",
    )
    plt.errorbar(
        algorithm2_avg_query.index,
        algorithm2_avg_query,
        yerr=algorithm2_std_query,
        color=algorithm2_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm2_color,
        markerfacecolor=algorithm2_color,
        label=algorithm2_name + " Query Time (us)",
    )
    plt.errorbar(
        algorithm3_avg_query.index,
        algorithm3_avg_query,
        yerr=algorithm3_std_query,
        color=algorithm3_color,
        marker="o",
        markersize=5,
        markeredgecolor=algorithm3_color,
        markerfacecolor=algorithm3_color,
        label=algorithm3_name + " Query Time (us)",
    )

    # Create a table with the query answer points
    table_data = []
    for N, points in query_answer_points.items():
        table_data.append([N, ", ".join(map(str, points))])

    # Add the table to the top right corner of the plot
    plt.table(
        cellText=table_data,
        colLabels=["N", "Query Answer Points"],
        loc="upper right",
        bbox=[0.65, 0.6, 0.3, 0.3],
        cellLoc="center",
    )

    plt.xlabel("N")
    plt.ylabel("Query Time (us)")
    plt.title("N vs. Query Time")
    plt.legend()
    plt.show()


def iterations_plot():
    # # Plotting N vs. iterations
    # plt.plot(n_values_1, iterations_1, 'r', label='Algorithm 1')
    # plt.plot(n_values_2, iterations_2, 'g', label='Algorithm 2')
    # plt.plot(n_values_3, iterations_3, 'b', label='Algorithm 3')
    # plt.xlabel('N')
    # plt.ylabel('Iterations')
    # plt.title('N vs. Iterations')
    # plt.legend()
    # plt.show()

    pass


if __name__ == "__main__":

    build_times = pd.read_csv(f"{prefix}build_times.csv")

    # Read the build_times and query_times csv files
    query_times = pd.read_csv(f"{prefix}query_times.csv")

    # Calculate the sum of sorting time and build time
    build_times["Total Time (us)"] = (
        build_times["Sorting Time (us)"] + build_times["Build Time (us)"]
    )

    algorithm1_name = "Brute force"
    algorithm2_name = "KD-tree"
    algorithm3_name = "Range tree"

    # Create separate data frames for each algorithm
    algorithm1 = build_times[build_times["Algorithm"] == algorithm1_name]
    algorithm2 = build_times[build_times["Algorithm"] == algorithm2_name]
    algorithm3 = build_times[build_times["Algorithm"] == algorithm3_name]

    # Set the color for each algorithm
    algorithm1_color = "r"
    algorithm2_color = "g"
    algorithm3_color = "b"

    build_time_plot(algorithm1, algorithm2, algorithm3, algorithm1_name, algorithm2_name, algorithm3_name, algorithm1_color, algorithm2_color, algorithm3_color)

    # Create separate data frames for each algorithm
    algorithm1_query = query_times[query_times["Algorithm"] == algorithm1_name]
    algorithm2_query = query_times[query_times["Algorithm"] == algorithm2_name]
    algorithm3_query = query_times[query_times["Algorithm"] == algorithm3_name]

    # Calculate the average query time and standard deviation for each N
    algorithm1_avg_query = algorithm1_query.groupby("N")["Time (us)"].mean()
    algorithm1_std_query = algorithm1_query.groupby("N")["Time (us)"].std()
    algorithm2_avg_query = algorithm2_query.groupby("N")["Time (us)"].mean()
    algorithm2_std_query = algorithm2_query.groupby("N")["Time (us)"].std()
    algorithm3_avg_query = algorithm3_query.groupby("N")["Time (us)"].mean()
    algorithm3_std_query = algorithm3_query.groupby("N")["Time (us)"].std()

    query_answer_points = {}
    for N, points in zip(algorithm1_query["N"], algorithm1_query["Points"]):
        try:
            query_answer_points[N].append(points)
        except:
            query_answer_points[N] = [points]

    query_time_plot(
        algorithm1_avg_query,
        algorithm1_std_query,
        algorithm2_avg_query,
        algorithm2_std_query,
        algorithm3_avg_query,
        algorithm3_std_query,
        algorithm1_name,
        algorithm2_name,
        algorithm3_name,
        algorithm1_color,
        algorithm2_color,
        algorithm3_color,
        query_answer_points,
    )
