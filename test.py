import os
import subprocess
import time

def run_and_capture_output(executable, input_file, timeout):
    try:
        start_time = time.time()
        process = subprocess.Popen(
            [executable],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )

        # Write the contents of the input file to the stdin of the process
        with open(input_file, 'r') as f:
            input_data = f.read()
            stdout, _ = process.communicate(input_data, timeout=timeout)

        elapsed_time = time.time() - start_time
        return process.returncode, stdout.strip(), elapsed_time
    except subprocess.TimeoutExpired:
        return 124, None, timeout

def main():
    if len(sys.argv) != 2:
        print("Usage: python test.py <executable>")
        return

    executable = sys.argv[1]

    for subtask in [1,2,3,4]:
        subtask_folder = f"tests/subtask{subtask}" 
        if not os.path.exists(subtask_folder):
            continue
        def solve_subtask():
            print(f"Subtask {subtask}:")
            input_files = sorted(os.listdir(subtask_folder))
            for input_file in input_files:
                if not input_file.startswith("input"):
                    continue

                test_num = input_file.replace("input", "").replace(".txt", "")
                output_file = os.path.join(subtask_folder, f"output{test_num}.txt")

                if not os.path.exists(output_file):
                    print(f"Output file {output_file} not found. Test failed.")
                    return

                run = lambda: run_and_capture_output(executable, os.path.join(subtask_folder, input_file), timeout=1) 
                run()# practice run?
                exit_code, output, elapsed_time = run()

                if exit_code == 124:
                    print(f"Time limit of 1 second exceeded on {input_file}. Test failed.")
                    return

                with open(output_file, 'r') as f:
                    expected_output = f.read().strip()

                if output  != expected_output:
                    print(f"Test failed on {input_file}.")
                    return
            print("All tests pass.")
        
        solve_subtask()

if __name__ == "__main__":
    import sys
    main()
