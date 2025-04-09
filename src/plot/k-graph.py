import pandas as pd
import glob
import matplotlib.pyplot as plt

# Caminho da pasta com os arquivos CSV
caminho = "../output/full/random/best-k/*.csv" 

# Lista para armazenar os DataFrames
dfs = []

# Lê e concatena todos os arquivos CSV
for arquivo in glob.glob(caminho):
    df = pd.read_csv(arquivo)
    dfs.append(df)

# Junta todos em um único DataFrame
df_total = pd.concat(dfs, ignore_index=True)

# Agrupa por 'k' e calcula a média do tempo
agrupado = df_total.groupby("k")["time_ms"].mean().reset_index()

# Plotando
plt.plot(agrupado["k"], agrupado["time_ms"], marker='o')
plt.xlabel("k")
plt.ylabel("Média de tempo (ms)")
plt.title("Tempo médio em função de k")
plt.grid(True)
plt.tight_layout()
plt.show()
