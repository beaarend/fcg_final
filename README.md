# INF01047: Fundamentos de Computação Gráfica
## Olimpíadas do Faustão

Este repositório contém o código e a documentação do trabalho desenvolvido para a disciplina de Fundamentos de Computação Gráfica. Como projeto final, criamos o jogo **Olímpiadas do Faustão**, inspirado no quadro do programa Domingão do Faustão, utilizando os conceitos aprendidos na disciplina.

---

## Integrantes do Grupo

| Nome                 | Cartão de Matrícula | 
| -------------------- | --------------------- | 
| Beatriz Aline Arend  | 00343354              | 
| João Carlos Eggers Fleck     | 00332794             | 


### Contribuições

- **Beatriz**  
  Responsável pelo mapeamento de texturas, criação da cutscene e implementação das câmeras.

- **João**  
  Responsável pela implementação de colisão entre objetos e carregamento de modelos 3D no formato `.obj`.

---

## Uso de Ferramentas de IA no Desenvolvimento

Utilizamos o **GitHub Copilot**, baseado no ChatGPT, como ferramenta auxiliar durante o desenvolvimento.  

- **Contribuições da ferramenta:**  
  - Foi útil para identificar e corrigir erros de código, especialmente na implementação das câmeras.  
  - Mostrou limitações ao lidar com shaders, frequentemente gerando código incorreto ou quebrando partes do programa.
 
Embora o **Copilot** tenha se mostrado útil em algumas partes do trabalho, como no debugging, ele foi menos eficaz para lidar com aspectos mais específicos e complexos, como shaders. Apesar disso, sua utilização acelerou algumas etapas e ajudou a identificar problemas sutis que passariam despercebidos.

---

## Descrição do Processo de Desenvolvimento

1. **Implementação inicial das câmeras:**  
   As câmeras foram configuradas com base no conteúdo estudado.

2. **Carregamento de modelos 3D:**  
   Utilizamos objetos no formato `.obj` sem texturas iniciais.  

3. **Mapeamento de texturas:**  
   Após o carregamento dos modelos, aplicamos texturas aos objetos.

4. **Colisão entre objetos:**  
   Implementamos um sistema de colisão para garantir que os objetos interagissem de forma coerente.

5. **Animação da câmera na cutscene:**  
   Para a animação da câmera na cutscene, utilizamos a curva de Bézier.

---

## Imagens da Aplicação

### 1. Cutscene  
![Cena com Texturas](/resources/relatorio/cutscene_relatorio.png)

### 2. Faustão de costas desviando de inimigos
![Animação Cutscene](/resources//relatorio//faustao_relatorio.png)

---

## Manual de Utilização

Os seguintes comandos podem ser utilizados para interagir com a aplicação:

- **W** - Move para frente  
- **A** - Move para a esquerda  
- **S** - Move para trás  
- **D** - Move para a direita  
- **SPACE** - Pula  

---

## Como Compilar e Executar a Aplicação

Siga os passos abaixo para compilar e executar a aplicação:

1. Clone o repositório:  
   ```bash
   git clone https://github.com/beaarend/fcg_final
2. Crie uma pasta para a build:
   ```bash
   mkdir build
   cd build
3. Configure o projeto com CMake:
   ```bash
   cmake ..
4. Compile e execute:
   ```bash
   make run
