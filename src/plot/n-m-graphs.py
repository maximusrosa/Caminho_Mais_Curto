import pandas as pd
import glob
import matplotlib.pyplot as plt

# Caminho da pasta com os arquivos CSV
caminho = "full/random/m-const/"  # <-- Altere aqui


if __name__ == "__main__":
    pares = []

    for arquivo in glob.glob(caminho + "*.csv"):
        # Lê o arquivo CSV
        df = pd.read_csv(arquivo)
        
        avgNormTime = df["normTime"].mean()
        
        numEdges = df["edges"].iloc[0]  
        numVertices = df["vertices"].iloc[0]
        
        pares.append((numVertices, avgNormTime))

    # Plotando os dados a partir da lista de pares
    # Desempacotando os pares
    x, y = zip(*pares)
    plt.scatter(x, y)
    plt.xlabel("Número de vértices")
    plt.ylabel("Média de tempo normalizado")
    plt.title("Média de tempo normalizado vs Número de vértices")
    plt.grid()
    plt.show()


    print(pares)