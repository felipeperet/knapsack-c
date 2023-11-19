import os
import re
import matplotlib.pyplot as plt
import pandas as pd

filename_pattern = re.compile(r"output-(\d+)-(n|W)\.txt")

def extract_time(content):
    match = re.search(r"Tempo de execução: ([\d.]+) segundos", content)
    return float(match.group(1)) if match else None

def read_times(directory, file_ending):
    times = {}
    for filename in os.listdir(directory):
        if filename.endswith(f"-{file_ending}.txt"):
            path = os.path.join(directory, filename)
            with open(path, 'r') as file:
                content = file.read()
                time = extract_time(content)
                size = filename_pattern.search(filename).group(1)
                times[int(size)] = time
    return times

def plot_data(times_dict, approach, file_ending):
    # Create the 'data/plots' directory if it doesn't exist
    os.makedirs('data/plots', exist_ok=True)
    if times_dict:
        df = pd.DataFrame({
            'Tamanho': sorted(times_dict.keys()),
            'Tempo': [times_dict[k] for k in sorted(times_dict.keys())]
        })

        plt.figure(figsize=(10, 5))
        plt.plot('Tamanho', 'Tempo', data=df, marker='', color='blue', linewidth=2)
        plt.xlabel('Tamanho da Instância')
        plt.ylabel('Tempo de Execução (segundos)')
        plt.title(f'Tempo de Execução - {approach} - {file_ending}')
        plt.legend([f'{approach} - {file_ending}'])

        # Adjust the filename to not include directories
        filename = f"grafico_{approach.replace('/', '_')}_{file_ending}.png"
        plt.savefig(os.path.join('data/plots', filename))
        plt.close()
    else:
        msg = (f"Não há dados para plotar para {approach.split('/')[-1]} "
               f"com arquivos terminando em '{file_ending}'.")
        print(msg)

# Assuming your script is now in the root directory,
# adjust the paths accordingly
approaches = ['backtracking', 'branch_and_bound', 'dynamic']
for approach in approaches:
    approach_path = f'data/outputs/{approach}'
    for file_ending in ['n', 'W']:
        times = read_times(approach_path, file_ending)
        plot_data(times, approach, file_ending)
