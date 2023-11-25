import os
import subprocess

# Get the current path of the Python script
script_path = os.path.abspath(__file__)
script_directory = os.path.dirname(script_path)

commited = True
commit_message = input("Enter commit message: ")

# Delete .exe files in subfolders of the script's directory
for root, dirs, files in os.walk(script_directory):
    for file in files:
        if file.endswith('.exe'):
            file_path = os.path.join(root, file)
            try:
                os.remove(file_path)
                print(f"Deleted: {file_path}")
            except Exception as e:
                print(f"Error deleting {file_path}: {e}")

# Git commands
commands = [
    'git add .',
    'git status',
    'git pull',
    f'git commit -m "{commit_message}"',
    'git push'
]

for command in commands:
    try:
        subprocess.run(command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
        commited = False
        print(f"Error running command: {e.cmd}")

if commited:
    print("\n>> Changes have been pushed successfully..!!")
