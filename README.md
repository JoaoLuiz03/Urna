<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>README - Urna Eletrônica</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #0d0d0d;
      color: #eaeaea;
      max-width: 960px;
      margin: 0 auto;
      padding: 40px 20px;
    }

    h1, h2, h3 {
      color: #00eaff;
    }

    p, li {
      font-size: 1rem;
    }

    a {
      color: #ffd700;
      text-decoration: none;
    }

    a:hover {
      text-decoration: underline;
    }

    .badge {
      display: inline-block;
      padding: 6px 12px;
      margin: 6px 6px 6px 0;
      border-radius: 5px;
      font-weight: bold;
      color: white;
    }

    .html { background-color: #e34c26; }
    .css { background-color: #1572b6; }
    .js { background-color: #f7df1e; color: #000; }
    .c { background-color: #00599c; }
    .git { background-color: #f1502f; }

    code, pre {
      background-color: #1c1c1c;
      color: #00ff88;
      padding: 10px;
      border-radius: 6px;
      display: block;
      overflow-x: auto;
    }

    ul {
      margin-left: 20px;
    }

    .footer {
      text-align: right;
      font-size: 0.9rem;
      margin-top: 40px;
      color: #777;
    }

    hr {
      border: 1px solid #333;
      margin: 40px 0;
    }
  </style>
</head>
<body>

  <h1>🗳️ Projeto Urna Eletrônica</h1>

  <p>Simulação de uma urna eletrônica desenvolvida em linguagem C. O sistema permite que usuários votem para Prefeito e Vereador, valida CPF, contabiliza votos e mostra os resultados com base em arquivos de entrada/saída.</p>

  <hr />

  <h2>📌 Funcionalidades</h2>
  <ul>
    <li>Votação por CPF</li>
    <li>Cadastro via arquivos de texto</li>
    <li>Votos válidos, brancos e nulos</li>
    <li>Apuração de resultados</li>
    <li>Limpeza dos arquivos de votação</li>
  </ul>

  <hr />

  <h2>🧾 Estrutura de Arquivos</h2>
  <ul>
    <li><code>main.c</code> — Código principal</li>
    <li><code>eleitores.txt</code> — Lista de eleitores válidos (CPF e nome)</li>
    <li><code>candidatos_prefeito.txt</code> e <code>candidatos_vereador.txt</code> — Dados dos candidatos</li>
    <li><code>votos_prefeito.txt</code> e <code>votos_vereador.txt</code> — Registro de votos</li>
  </ul>

  <hr />

  <h2>🛠️ Tecnologias Usadas</h2>
  <span class="badge c">C</span>
  <span class="badge git">Git</span>

  <hr />

  <h2>▶️ Como Executar</h2>
  <p>Compile com <code>gcc</code> ou qualquer compilador C:</p>
  <pre><code>gcc main.c -o urna
./urna</code></pre>

  <p>O programa exibirá um menu interativo para votar, apurar ou limpar os dados.</p>

  <hr />

  <h2>📂 Exemplo de Dados</h2>

  <h3><code>eleitores.txt</code></h3>
  <pre><code>12345678901;João da Silva
98765432100;Maria Oliveira</code></pre>

  <h3><code>candidatos_prefeito.txt</code></h3>
  <pre><code>10;Ana Costa;Partido Azul
20;Carlos Lima;Partido Verde</code></pre>

  <h3><code>candidatos_vereador.txt</code></h3>
  <pre><code>1010;Pedro Rocha;Partido Roxo
2020;Fernanda Luz;Partido Amarelo</code></pre>

  <hr />

  <div class="footer">
    Feito por <a href="https://github.com/JoaoLuiz03" target="_blank">João Luiz</a> 💻
  </div>

</body>
</html>
