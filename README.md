# 🗳️ Projeto Urna Eletrônica

Simulação de uma urna eletrônica desenvolvida em linguagem C. O sistema permite que usuários votem para Prefeito e Vereador, valida CPF, contabiliza votos e mostra os resultados com base em arquivos de entrada/saída.

---

## 📌 Funcionalidades

- Votação por CPF
- Cadastro via arquivos de texto
- Votos válidos, brancos e nulos
- Apuração de resultados
- Limpeza dos arquivos de votação

---

## 🧾 Estrutura de Arquivos

- `main.c` — Código principal  
- `eleitores.txt` — Lista de eleitores válidos (CPF e nome)  
- `candidatos_prefeito.txt` e `candidatos_vereador.txt` — Dados dos candidatos  
- `votos_prefeito.txt` e `votos_vereador.txt` — Registro de votos

---

## 🛠️ Tecnologias Usadas

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Git](https://img.shields.io/badge/Git-F1502F?style=for-the-badge&logo=git&logoColor=white)

---

## ▶️ Como Executar

Compile com `gcc` ou qualquer compilador C:

```bash
gcc main.c -o urna
./urna
