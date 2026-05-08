#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

//  Quadro — um recorte do spritesheet + sua duração
struct Quadro {
    sf::IntRect regiao;   // retângulo (x, y, w, h) no spritesheet
    float       duracao;  // duração em segundos (ex: 0.12f = ~8 fps)
};

//  AnimacaoSprite — gerencia uma sequência de quadros
class AnimacaoSprite {
public:
    AnimacaoSprite();

    // Adiciona um quadro (recorte + duração)
    void adicionarQuadro(int x, int y, int w, int h, float duracao = 0.12f);
    void adicionarQuadro(sf::IntRect regiao, float duracao = 0.12f);

    // Configurações
    void setLoop(bool loop); // se true, reinicia ao terminar
    void setEscala(float sx, float sy);

    // Controle
    void reiniciar();

    // Atualiza o timer; retorna true quando a animação NÃO-LOOP termina
    bool atualizar(float dt);

    // Aplica o quadro atual ao sf::Sprite
    void aplicar(sf::Sprite& sprite) const;

    bool isConcluida()  const;
    bool isEmpty()      const;
    int  totalQuadros() const;

private:
    std::vector<Quadro> quadros;
    int   quadroAtual;
    float tempoCorrido;
    bool  loop;
    bool  concluida;
    float escX, escY;
};
