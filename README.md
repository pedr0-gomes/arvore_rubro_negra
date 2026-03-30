# arvore-rubro-negra

> Implementação de inserção e remoção em Árvore Rubro-Negra Direitista em C, com abordagem iterativa e ponteiro pai.

## Sobre o projeto

Referência teórica: [`docs/aula21-arvores-rubro-negras.pdf`](docs/aula21-arvores-rubro-negras.pdf)

Repositório desenvolvido na disciplina de Estruturas de Informação, com base na Aula 21 — Árvores Rubro-Negras. O objetivo é implementar as operações de inserção e remoção em uma Right-Leaning Red-Black Tree (LLRB), usando uma abordagem iterativa com ponteiro pai em cada nó e um nó sentinela NIL global compartilhado.

## Estrutura do repositório
```
arvore_rubro_negra/
├── .gitignore               — arquivos ignorados pelo Git
├── LICENSE                  — licença MIT
├── README.md                — este arquivo
├── rbt.h                    — interface pública do TAD
├── rbt.c                    — implementação do TAD base
├── Makefile                 — regras de compilação
├── template_insercao.c      — template com harness de teste para inserção
├── template_remocao.c       — template com harness de teste para remoção
├── insercao.c               — exercício: inserção com balanceamento
└── remocao.c                — exercício: remoção com rebalanceamento
```

## Como compilar e executar

**Ambiente:** Windows 11 + PowerShell + GCC via MinGW
```powershell
make insercao.exe
.\insercao.exe

make remocao.exe
.\remocao.exe
```

## Implementação

O TAD (`rbt.h` / `rbt.c`) define a estrutura do nó com os campos `val`, `esq`, `dir`, `pai` e `cor`, além do sentinela NIL estático que representa todas as folhas ausentes. As rotações (`rotacao_esq`, `rotacao_dir`) atualizam os ponteiros pai dos três nós envolvidos e são usadas tanto na inserção quanto na remoção.

A inserção segue as 4 regras da nota de aula: o novo nó entra vermelho, a subida aplica rotação à direita para corrigir filho esquerdo vermelho, rotação à esquerda para desfazer a estrutura P-V-V, e a raiz é sempre pintada de preto ao final.

A remoção localiza o nó, faz a remoção física com substituição pelo sucessor em ordem quando necessário, e executa o rebalanceamento pela subida com os casos simétricos de déficit de preto.

## Aprendizados

- Fluxo completo de engenharia de software: branch → implementação → validação → PR → merge → limpeza
- A diferença prática entre implementação recursiva e iterativa com ponteiro pai — e como o ponteiro pai torna explícito o caminho de subida que na recursão é implícito na pilha
- Uso de nó sentinela NIL para eliminar verificações de NULL e centralizar invariantes da estrutura

## Status

Concluído — inserção e remoção implementadas e validadas.