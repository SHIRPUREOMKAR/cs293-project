import subprocess
# from datetime import datetime

# current_time = datetime.now().strftime('%H:%M:%S %Y-%m-%d')
commited = True
commit_message = input("Enter commit message : ")

commands = [
    'git add .',
    'git status',
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
    print()
    print(f">> Changes have been pushed successfully..!!")
