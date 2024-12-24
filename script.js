// Função para buscar Pokémon
async function buscarPokemon() {
    const idInput = document.getElementById('pokemon-id');
    const resultadoDiv = document.getElementById('resultado');
    const id = idInput.value.trim();
    console.log(idInput.value);

    // Limpar o resultado anterior
    resultadoDiv.innerHTML += '';

    if (!id) {
        resultadoDiv.innerHTML = '<p class="error">Por favor, digite um ID válido.</p>';
        return;
    }

    try {
        // Fazer a requisição para a API
        const response = await fetch(`http://localhost:8080/pokemon?id=${id}`);
        
        if (!response.ok) {
            throw new Error(`Erro: ${response.statusText}`);
        }

        // Parse do JSON recebido
        const data = await response.json();

        // Exibir as informações do Pokémon
        resultadoDiv.innerHTML += `
            <div class="pokemon-card">
                <img src="/PokemonIMG/${data.name.toLowerCase()}.png" alt="Minha Figura">
                <h2>${data.name} (#${data.id})</h2>
                <p><strong>Descrição:</strong> ${data.description}</p>
                <p><strong>Tipo:</strong> ${data.type}</p>
                <p><strong>Habilidades:</strong> ${data.abilities}</p>
                <p><strong>Peso:</strong> ${data.weight} kg</p>
                <p><strong>Altura:</strong> ${data.height} m</p>
                <p><strong>Taxa de Captura:</strong> ${data.capture_rate}</p>
                <p><strong>É Lendário:</strong> ${data.is_legendary ? 'Sim' : 'Não'}</p>
                <p><strong>Geração:</strong> ${data.generation}</p>
                <p><strong>Data de Captura:</strong> ${data.capture_date}</p>
            </div>
        `;
    } catch (error) {
        resultadoDiv.innerHTML = `<p class="error">Erro ao buscar Pokémon: ${error.message}</p>`;
    }
}

// Adicionar evento ao botão
document.getElementById('buscar-pokemon').addEventListener('click', buscarPokemon);