import subprocess
from pathlib import Path

from large_file_generator import generate

server = 'vcf3'
server_port = 4000
receiver = 'vcf4'
sender = 'vcf5'
home_dir = Path('/home/ugrads/dluo')
project_path = home_dir / 'notes/CSCI_4760/pj03'

large_file = project_path / 'test_files/large_file.raw'
if not large_file.is_file():
    generate(open(large_file, 'wb'), 1 << 30)
target_file = project_path / 'large_file.raw'

server_proc = subprocess.Popen(
    f"ssh -t dluo@{server} "
    f"'cd {project_path.absolute()}; "
    f"source {home_dir / '.bash_login'}; "
    f"python3 ftserver.py {server_port}'".split()
)

power_range = range(10, 11)
conn_range = range(1)

with open('results.csv', 'w') as f:
    for power in power_range:
        for num_connections in conn_range:
            if target_file.is_file():
                target_file.unlink()

            size = 1 << power

            receiver_proc = subprocess.Popen(
                f"ssh -t dluo@{server} "
                f"'cd {project_path.absolute()}; "
                f"source {home_dir / '.bash_login'}; "
                f"python3 ftclient.py --receive 1 --server {server}:{server_port}' -c {num_connections} -s {size} --log-level SCRIPT".split(),
                stdout=subprocess.PIPE
            )

            id = receiver_proc.stdout.readline()

            sender_proc = subprocess.Popen(
                f"ssh dluo@{server} "
                f"'cd {project_path.absolute()}; "
                f"source {home_dir / '.bash_login'}; "
                f"python3 ftclient.py --send {id} {large_file} --server {server}:{server_port}'".split(),
                stdout=subprocess.PIPE
            )

            data = receiver_proc.stdout.readline()
            values = data.split(':', maxsplit=2)[2]
            f.write(values + '\n')
