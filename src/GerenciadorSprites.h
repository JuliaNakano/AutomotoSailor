#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "automato.h"
#include "AnimacaoSprite.h"

class GerenciadorSprites {
public:
    GerenciadorSprites();

    bool carregar(const std::string& caminho);

    bool atualizar(Estado estado, float dt, sf::Sprite& sprite);

    void trocarEstado(Estado novoEstado);

    const sf::Texture& getTextura() const;

private:
    sf::Texture textura;
    std::map<Estado, AnimacaoSprite> animacoes;
    Estado estadoAnterior;

    void inicializarAnimacoes();

    // Helpers de construção
    AnimacaoSprite& criarAnimacao(Estado estado, bool loop = true);
};
