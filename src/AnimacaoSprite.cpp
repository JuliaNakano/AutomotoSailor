#include "AnimacaoSprite.h"

AnimacaoSprite::AnimacaoSprite()
    : quadroAtual(0), tempoCorrido(0.f),
      loop(true), concluida(false),
      escX(1.f), escY(1.f) {}

//Adicionar quadros

void AnimacaoSprite::adicionarQuadro(int x, int y, int w, int h, float duracao) {
    quadros.push_back({ sf::IntRect(x, y, w, h), duracao });
}

void AnimacaoSprite::adicionarQuadro(sf::IntRect regiao, float duracao) {
    quadros.push_back({ regiao, duracao });
}

//Configurações

void AnimacaoSprite::setLoop(bool l) { loop = l; }
void AnimacaoSprite::setEscala(float sx, float sy) { escX = sx; escY = sy; }

// Controle

void AnimacaoSprite::reiniciar() {
    quadroAtual  = 0;
    tempoCorrido = 0.f;
    concluida    = false;
}

//Atualização

bool AnimacaoSprite::atualizar(float dt) {
    if (concluida || quadros.empty()) return concluida;

    tempoCorrido += dt;

    // Avança quadros enquanto o tempo acumulado ultrapassar a duração do quadro atual
    while (tempoCorrido >= quadros[quadroAtual].duracao) {
        tempoCorrido -= quadros[quadroAtual].duracao;
        quadroAtual++;

        if (quadroAtual >= (int)quadros.size()) {
            if (loop) {
                quadroAtual = 0;
            } else {
                // Congela no último quadro e sinaliza conclusão
                quadroAtual = (int)quadros.size() - 1;
                concluida   = true;
                return true;   // Sinal para o Jogo enviar ANIMACAO_CONCLUIDA
            }
        }
    }
    return false;
}

// Aplicação ao Sprite

void AnimacaoSprite::aplicar(sf::Sprite& sprite) const {
    if (!quadros.empty()) {
        sprite.setTextureRect(quadros[quadroAtual].regiao);
        sprite.setScale(escX, escY);
    }
}

// Getters

bool AnimacaoSprite::isConcluida() const { return concluida; }
bool AnimacaoSprite::isEmpty() const { return quadros.empty(); }
int  AnimacaoSprite::totalQuadros() const { return (int)quadros.size(); }
