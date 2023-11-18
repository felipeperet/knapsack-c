CC=gcc
CFLAGS=-Wall -g
SRCDIR=src
INCDIR=include
OBJDIR=obj
BINDIR=bin
EXE=$(BINDIR)/knapsack
GENERATE_INPUTS_SRC=$(SRCDIR)/generate_inputs.c
GENERATE_INPUTS_EXE=$(BINDIR)/generate_inputs

# Lista de arquivos fonte para o executável principal, excluindo generate_inputs.c
SOURCES=$(filter-out $(GENERATE_INPUTS_SRC),$(wildcard $(SRCDIR)/*.c))
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Alvo padrão para construir o projeto knapsack
all: directories $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJECTS)

# Regra de padrão para construir arquivos objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Cria os diretórios necessários
directories: | $(BINDIR) $(OBJDIR)

$(BINDIR) $(OBJDIR):
	mkdir -p $@

# Alvo para construir e executar o gerador de entradas
generate-inputs:
	$(CC) $(CFLAGS) -I$(INCDIR) -o $(GENERATE_INPUTS_EXE) $(GENERATE_INPUTS_SRC)
	@echo "Executando o gerador de entradas..."
	@$(GENERATE_INPUTS_EXE)
	@echo "Removendo arquivos objeto..."
	@rm -f $(GENERATE_INPUTS_EXE)

# Alvo phony para limpeza do projeto
.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(EXE)

# Alvo phony para executar o knapsack
.PHONY: run
run:
	@$(EXE)
