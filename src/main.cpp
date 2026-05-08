// ============================================================
//  main.cpp  —  Jogo principal com SFML
//
//  MAPEAMENTO DE TECLAS:
//  ┌────────────┬─────────────────────────────────────────┐
//  │ Tecla      │ Ação                                    │
//  ├────────────┼─────────────────────────────────────────┤
//  │ D          │ Andar (→ ANDANDO)                       │
//  │ S          │ Parar (→ PARADO)                        │
//  │ W          │ Pular vertical (→ PULANDO)              │
//  │ E          │ Pular para frente (→ PULO_FRENTE)       │
//  │ J          │ Soco (→ SOCO)                           │
//  │ K          │ Soco novamente / Combo (→ COMBO)        │
//  │ U          │ Chute no ar (→ CHUTE_AEREO)             │
//  │ I          │ Soco descida no ar (→ SOCO_DESCIDA)     │
//  │ Space      │ Especial / Super poder (→ ESPECIAL)     │
//  │ G          │ Segurar inimigo (→ SEGURAR)             │
//  │ T          │ Tapa enquanto segura (→ TAPA_SEGURADO)  │
//  │ R          │ Arremessar (→ ARREMESSO)                │
//  │ H          │ Tomar hit (demo)                        │
//  │ F          │ Tomar hit forte (demo)                  │
//  │ Z          │ Tomar hit fatal / Derrota (demo)        │
//  │ V          │ Vencer (demo)                           │
//  │ Backspace  │ Resetar autômato                        │
//  └────────────┴─────────────────────────────────────────┘
//
// ============================================================

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "automato.h"
#include "GerenciadorSprites.h"

// Configurações da janela
static constexpr unsigned LARGURA_JANELA  = 800;
static constexpr unsigned ALTURA_JANELA   = 400;
static constexpr unsigned ESCALA_SPRITE   = 3;    // zoom do personagem
static constexpr float    DURACAO_PULO    = 1.0f; // s no ar antes de aterrissar

// Caminho do spritesheet (ajuste se necessário)
static const std::string SPRITESHEET_PATH = "assets/sprites/SpriteSheet.jpeg";

int main() {

    // Crindo Janela
    sf::RenderWindow janela(
        sf::VideoMode(LARGURA_JANELA, ALTURA_JANELA),
        "Sailor Moon – Automato Finito (SFML)",
        sf::Style::Close
    );
    janela.setFramerateLimit(60);

    // Automato
    Automato automato;

    // Sprites
    GerenciadorSprites gerSprites;
    if (!gerSprites.carregar(SPRITESHEET_PATH)) {
        std::cerr << "Nao foi possivel abrir " << SPRITESHEET_PATH << "\n"
                  << "Certifique-se de que o arquivo esta na pasta do executavel.\n";
        return 1;
    }

    sf::Sprite sprite;
    sprite.setTexture(gerSprites.getTextura());
    sprite.setScale(ESCALA_SPRITE, ESCALA_SPRITE);

    // Posição central inferior da janela
    sprite.setPosition(
        LARGURA_JANELA / 2.f - 30.f * ESCALA_SPRITE,
        ALTURA_JANELA  - 85.f * ESCALA_SPRITE - 10.f
    );

    // Fundo colorido
    sf::RectangleShape fundo(sf::Vector2f(LARGURA_JANELA, ALTURA_JANELA));
    fundo.setFillColor(sf::Color(20, 20, 60));  // azul noturno

    // HUD (texto de estado)
    sf::Font fonte;
    bool fonteOk = fonte.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");
    if (!fonteOk)
        fonteOk = fonte.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf");

    sf::Text textoEstado;
    if (fonteOk) {
        textoEstado.setFont(fonte);
        textoEstado.setCharacterSize(18);
        textoEstado.setFillColor(sf::Color::Yellow);
        textoEstado.setPosition(10.f, 10.f);
    }

    sf::Text textoControles;
    if (fonteOk) {
        textoControles.setFont(fonte);
        textoControles.setCharacterSize(12);
        textoControles.setFillColor(sf::Color(200, 200, 200));
        textoControles.setPosition(10.f, ALTURA_JANELA - 60.f);
        textoControles.setString(
            "D=andar  S=parar  W=pular  E=pular-frente\n"
            "J=soco  K=combo  U=chute-ar  I=soco-ar\n"
            "Space=ESPECIAL  G=segurar  T=tapa  R=arremesso\n"
            "H=hit  F=hit-forte  Z=fatal  V=vencer  BkSp=reset"
        );
    }

    // Timer de pulo (auto-aterrissar)
    float timerPulo     = 0.f;
    bool  estaNoPulo    = false;

    Estado estadoAnterior = automato.getEstado();

    // Clock para delta-time
    sf::Clock relogio;

    // Lambda: enviar entrada ao autômato 
    auto enviar = [&](Entrada e) {
        automato.processar(e);
        Estado novo = automato.getEstado();
        if (novo != estadoAnterior) {
            gerSprites.trocarEstado(novo);
            estadoAnterior = novo;
        }
    };

    // Loop principal
    while (janela.isOpen()) {

        float dt = relogio.restart().asSeconds();
        // Limita dt para evitar saltos ao perder foco
        if (dt > 0.1f) dt = 0.1f;

        // Eventos
        sf::Event ev;
        while (janela.pollEvent(ev)) {

            if (ev.type == sf::Event::Closed)
                janela.close();

            if (ev.type == sf::Event::KeyPressed) {
                Estado est = automato.getEstado();
                bool emAr  = (est == Estado::PULANDO   ||
                              est == Estado::PULO_FRENTE ||
                              est == Estado::CHUTE_AEREO  ||
                              est == Estado::SOCO_DESCIDA);

                switch (ev.key.code) {

                    // Movimentação
                    case sf::Keyboard::D:
                        enviar(Entrada::ANDAR); break;

                    case sf::Keyboard::S:
                        enviar(Entrada::PARAR); break;

                    case sf::Keyboard::W:
                        enviar(Entrada::PULAR);
                        estaNoPulo = true;
                        timerPulo  = 0.f;
                        break;

                    case sf::Keyboard::E:
                        enviar(Entrada::PULAR_FRENTE);
                        estaNoPulo = true;
                        timerPulo  = 0.f;
                        break;

                    // Ataques básicos
                    case sf::Keyboard::J:
                        if (emAr) enviar(Entrada::CHUTE_NO_AR);
                        else      enviar(Entrada::SOCO);
                        break;

                    case sf::Keyboard::K:
                        enviar(Entrada::SOCO_NOVAMENTE); break;

                    case sf::Keyboard::U:
                        enviar(Entrada::CHUTE_NO_AR); break;

                    case sf::Keyboard::I:
                        enviar(Entrada::SOCO_NO_AR_BAIXO); break;

                    // Especial
                    case sf::Keyboard::Space:
                        enviar(Entrada::ESPECIAL); break;

                    // Segurar / Arremesso
                    case sf::Keyboard::G:
                        enviar(Entrada::SEGURAR_INIMIGO); break;

                    case sf::Keyboard::T:
                        enviar(Entrada::TAPA); break;

                    case sf::Keyboard::R:
                        enviar(Entrada::ARREMESSAR); break;

                    // Dano (demo)
                    case sf::Keyboard::H:
                        enviar(Entrada::TOMAR_HIT); break;

                    case sf::Keyboard::F:
                        enviar(Entrada::TOMAR_HIT_FORTE); break;

                    case sf::Keyboard::Z:
                        enviar(Entrada::TOMAR_HIT_FATAL); break;

                    // Vitória (demo)
                    case sf::Keyboard::V:
                        enviar(Entrada::VENCER); break;

                    // Reset
                    case sf::Keyboard::BackSpace:
                        automato.resetar();
                        gerSprites.trocarEstado(Estado::PARADO);
                        estadoAnterior = Estado::PARADO;
                        estaNoPulo = false;
                        break;

                    default: break;
                }
            }
        }

        // Lógica: auto-aterrissar após DURACAO_PULO
        Estado estAtual = automato.getEstado();
        bool emAr = (estAtual == Estado::PULANDO    ||
                     estAtual == Estado::PULO_FRENTE ||
                     estAtual == Estado::CHUTE_AEREO  ||
                     estAtual == Estado::SOCO_DESCIDA);

        if (emAr) {
            timerPulo += dt;
            if (timerPulo >= DURACAO_PULO) {
                timerPulo  = 0.f;
                estaNoPulo = false;
                enviar(Entrada::ATERRISSAR);
            }
        }

        // Atualizar animação; detectar ANIMACAO_CONCLUIDA
        bool animConcluida = gerSprites.atualizar(automato.getEstado(), dt, sprite);
        if (animConcluida) {
            enviar(Entrada::ANIMACAO_CONCLUIDA);
        }

        // HUD: texto do estado atual
        if (fonteOk)
            textoEstado.setString("Estado: " + automato.getNomeEstado());

        // Render
        janela.clear();
        janela.draw(fundo);
        janela.draw(sprite);
        if (fonteOk) {
            janela.draw(textoEstado);
            janela.draw(textoControles);
        }
        janela.display();
    }

    return 0;
}
