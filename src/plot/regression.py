import pandas as pd
import numpy as np
import glob
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.metrics import r2_score, mean_squared_error

# Caminho da pasta com os arquivos CSV
caminhos = ["full/random/m-const/", "full/random/n-const/"]

if __name__ == "__main__":
    logs = []
    reais = []
    previstos = []

    for caminho in caminhos:
        for arquivo in glob.glob(caminho + "*.csv"):
            df = pd.read_csv(arquivo)

            avgNormTime = df["normTime"].mean()
            n = df["vertices"].iloc[0]
            m = df["edges"].iloc[0]

            if avgNormTime > 0:
                logs.append((np.log(n), np.log(m), np.log(avgNormTime)))
                reais.append(avgNormTime)

    logs = np.array(logs)
    X = logs[:, :2]  # log(n), log(m)
    y_log = logs[:, 2]   # log(T)

    # Regressão linear
    reg = LinearRegression().fit(X, y_log)
    y_log_pred = reg.predict(X)

    # Converte de volta para escala original
    y_pred = np.exp(y_log_pred)

    # Erros
    r2 = r2_score(reais, y_pred)
    rmse = np.sqrt(mean_squared_error(reais, y_pred))

    # Parâmetros
    a_log = reg.intercept_
    b, c = reg.coef_
    a = np.exp(a_log)

    print(f"Modelo ajustado: T(n, m) ≈ {a:.5e} * n^{b:.3f} * m^{c:.3f}")
    print(f"R²: {r2:.4f}")
    print(f"RMSE: {rmse:.5e}")

    # Plot real vs previsto
    plt.scatter(reais, y_pred, color='blue', alpha=0.6)
    plt.plot([min(reais), max(reais)], [min(reais), max(reais)], 'r--', label='Previsão')
    plt.xlabel("Tempo normalizado real")
    plt.ylabel("Tempo previsto pelo modelo")
    plt.title("Tempo real vs Tempo previsto")
    plt.legend()
    plt.grid()
    plt.show()
