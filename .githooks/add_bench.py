import subprocess

subprocess.run(["git", "add", "bench.json"])
subprocess.run(["git", "commit", "-m", "chore: update benchmarks","--no-verify"])
subprocess.run(["git", "push"])