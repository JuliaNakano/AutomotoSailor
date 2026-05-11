#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "automato.h"
#include "AnimacaoSprite.h"

//  GerenciadorSprites
//  Carrega o spritesheet e mapeia cada Estado → AnimacaoSprite

class GerenciadorSprites {
public:
    GerenciadorSprites();

    // Carrega a textura do spritesheet.
    // Retorna false se o arquivo não for encontrado.
    bool carregar(const std::string& caminho);

    // Aplica o quadro atual da animação do estado ao sprite
    // Retorna true quando uma animação não-loop termina (→ ANIMACAO_CONCLUIDA)
    bool atualizar(Estado estado, float dt, sf::Sprite& sprite);

    // Troca de estado: reinicia a nova animação
    void trocarEstado(Estado novoEstado);

    // Acesso à textura (para setar no sf::Sprite externamente)
    const sf::Texture& getTextura() const;

private:
    sf::Texture textura;
    std::map<Estado, AnimacaoSprite> animacoes;
    Estado estadoAnterior;

    void inicializarAnimacoes();

    // Helpers de construção
    AnimacaoSprite& criarAnimacao(Estado estado, bool loop = true);
};
