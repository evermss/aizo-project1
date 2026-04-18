import os
import subprocess
import sys


APP_PATH = os.path.join("build", "aizo-project1")
RESULTS_DIR = os.path.join("results", "research")


def run_command(command):
    print(f"\n>>> {' '.join(command)}")
    result = subprocess.run(command)
    if result.returncode != 0:
        print("\nBlad podczas wykonywania komendy.")
        sys.exit(result.returncode)


def clear_csv_results():
    os.makedirs(RESULTS_DIR, exist_ok=True)
    for name in os.listdir(RESULTS_DIR):
        if name.endswith(".csv"):
            os.remove(os.path.join(RESULTS_DIR, name))


def main():
    clear_csv_results()

    run_command(["cmake", "-S", ".", "-B", "build"])
    run_command(["cmake", "--build", "build"])

    alpha_size = "25000"
    base_size = "25000"
    iterations = "30"

    jobs = []

    jobs.append((
        "alpha_quick",
        [
            APP_PATH,
            "--benchmark",
            "-a", "5",
            "-s", "0",
            "-t", "0",
            "-l", alpha_size,
            "-n", iterations,
            "-r", os.path.join(RESULTS_DIR, "alpha_results.csv"),
        ],
    ))

    jobs.append((
        "alpha_shell",
        [
            APP_PATH,
            "--benchmark",
            "-a", "6",
            "-s", "0",
            "-t", "0",
            "-l", alpha_size,
            "-n", iterations,
            "-r", os.path.join(RESULTS_DIR, "alpha_results.csv"),
        ],
    ))

    algorithms = [
        ("bucket", "4"),
        ("quick", "5"),
        ("shell", "6"),
    ]

    linear_structures = [
        ("array", "0"),
        ("single_list", "1"),
        ("double_list", "2"),
    ]

    for alg_name, alg_id in algorithms:
        for struct_name, struct_id in linear_structures:
            jobs.append((
                f"A_{alg_name}_{struct_name}",
                [
                    APP_PATH,
                    "--benchmark",
                    "-a", alg_id,
                    "-s", struct_id,
                    "-t", "0",
                    "-l", base_size,
                    "-n", iterations,
                    "-r", os.path.join(RESULTS_DIR, "A_results.csv"),
                ],
            ))

    for struct_name, struct_id in linear_structures:
        jobs.append((
            f"B_quick_{struct_name}",
            [
                APP_PATH,
                "--benchmark",
                "-a", "5",
                "-s", struct_id,
                "-t", "0",
                "-l", base_size,
                "-n", iterations,
                "-r", os.path.join(RESULTS_DIR, "B_results.csv"),
            ],
        ))

    jobs.append((
        "C_quick_array",
        [
            APP_PATH,
            "--benchmark",
            "-a", "5",
            "-s", "0",
            "-t", "0",
            "-l", base_size,
            "-n", iterations,
            "-r", os.path.join(RESULTS_DIR, "C_results.csv"),
        ],
    ))

    jobs.append((
        "omega_quick",
        [
            APP_PATH,
            "--benchmark",
            "-a", "5",
            "-s", "0",
            "-t", "0",
            "-l", base_size,
            "-n", iterations,
            "-r", os.path.join(RESULTS_DIR, "omega_results.csv"),
        ],
    ))

    for name, command in jobs:
        print(f"\n=== Uruchamianie badania: {name} ===")
        run_command(command)

    print("\nGotowe. Wyniki zapisano w folderze results/research.")


if __name__ == "__main__":
    main()