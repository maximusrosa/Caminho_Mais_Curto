import pandas as pd
import glob
import matplotlib.pyplot as plt

# Caminho da pasta com os arquivos CSV
caminho = "output/ny-usa/"  # <-- Altere aqui

if __name__ == "__main__":
    pares = []

    for arquivo in glob.glob(caminho + "*.csv"):
        # Lê o arquivo CSV
        df = pd.read_csv(arquivo)

        avgTimeMs = df["time_ms"].mean()
        avgMemKb = df["mem_kb"].mean()

        pares.append((avgTimeMs, avgMemKb))

    # Plotando os dados a partir da lista de pares
    # Desempacotando os pares
    x, y = zip(*pares)

    print(pares)
