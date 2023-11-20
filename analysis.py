import os
import re
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import scipy.stats as stats

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

        filename = f"grafico_{approach.replace('/', '_')}_{file_ending}.png"
        plt.savefig(os.path.join('data/plots', filename))
        plt.close()
    else:
        print(f"Não há dados para plotar para {approach.split('/')[-1]} com arquivos terminando em '{file_ending}'.")

def calculate_confidence_interval(times):
    mean = np.mean(times)
    std_err = stats.sem(times)
    h = std_err * stats.t.ppf((1 + 0.95) / 2, len(times) - 1)
    return mean, mean - h, mean + h

def write_parity_tests(times_dict, approach, file_ending):
    os.makedirs('data/tests', exist_ok=True)
    with open(os.path.join('data/tests', 'parity_tests.txt'), 'a') as file:
        file.write(f"Testes de paridade para {approach} com arquivos terminando em '{file_ending}':\n")
        for size, time in times_dict.items():
            file.write(f"Tamanho: {size}, Tempo: {time:.4f} segundos\n")
        if times_dict:
            times = list(times_dict.values())
            mean, lower, upper = calculate_confidence_interval(times)
            file.write(f"Intervalo de Confiança de 95%: Média = {mean:.4f}, Limite Inferior = {lower:.4f}, "
                       f"Limite Superior = {upper:.4f}\n")
        file.write("\n")

approaches = ['backtracking', 'branch_and_bound', 'dynamic']
for approach in approaches:
    approach_path = f'data/outputs/{approach}'
    for file_ending in ['n', 'W']:
        times = read_times(approach_path, file_ending)
        write_parity_tests(times, approach, file_ending)
        plot_data(times, approach, file_ending)
