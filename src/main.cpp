#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "automato.h"
#include "GerenciadorSprites.h"
#include "GerenciadorAudio.h"

//Configurações da janela
static constexpr unsigned LARGURA_JANELA = 800;
static constexpr unsigned ALTURA_JANELA  = 400;
static constexpr unsigned ESCALA_SPRITE  = 5;    // zoom do personagem
static constexpr float    DURACAO_PULO   = 1.0f; // s no ar antes de aterrissar

//Caminhos de assets
static const std::string SPRITESHEET_PATH = "assets/sprites/SpriteSheet.png";
static const std::string BACKGROUND_PATH  = "assets/backgrounds/Background.jpeg";
static const std::string MUSICA_PATH      = "assets/audio/musica.ogg";

int main() {

    //Janela
    sf::RenderWindow janela(
        sf::VideoMode(LARGURA_JANELA, ALTURA_JANELA),
        "Sailor Moon – Automato Finito (SFML)",
        sf::Style::Close
    );
    janela.setFramerateLimit(60);

    //Autômato
    Automato automato;

    //Spritesheet do personagem
    GerenciadorSprites gerSprites;
    if (!gerSprites.carregar(SPRITESHEET_PATH)) {
        std::cerr << "Nao foi possivel abrir " << SPRITESHEET_PATH << "\n"
                  << "Certifique-se de que o arquivo esta na pasta do executavel.\n";
        return 1;
    }

    sf::Sprite sprite;
    sprite.setTexture(gerSprites.getTextura());
    sprite.setScale(ESCALA_SPRITE, ESCALA_SPRITE);

    //Posição central-inferior da janela
    sprite.setPosition(
        LARGURA_JANELA / 2.f - 30.f * ESCALA_SPRITE,
        ALTURA_JANELA  - 40.f * ESCALA_SPRITE - 10.f
    );

    //Background
    sf::Texture texturaFundo;
    sf::Sprite  spriteFundo;
    bool        fundoComImagem = false;

    if (texturaFundo.loadFromFile(BACKGROUND_PATH)) {
        texturaFundo.setSmooth(true);

        sf::Vector2u szTex = texturaFundo.getSize();
        float escX = static_cast<float>(LARGURA_JANELA) / szTex.x;
        float escY = static_cast<float>(ALTURA_JANELA)  / szTex.y;

        spriteFundo.setTexture(texturaFundo);
        spriteFundo.setScale(escX, escY);

        fundoComImagem = true;
        std::cout << "[Fundo] Imagem carregada: " << BACKGROUND_PATH << "\n";
    } else {
        std::cout << "[Fundo] Imagem nao encontrada (" << BACKGROUND_PATH
                  << "); usando cor solida de fallback.\n";
    }

    sf::RectangleShape fundoFallback(sf::Vector2f(LARGURA_JANELA, ALTURA_JANELA));
    fundoFallback.setFillColor(sf::Color(20, 20, 60));

    //Música de fundo
    GerenciadorAudio gerAudio;
    if (gerAudio.carregar(MUSICA_PATH)) {
        gerAudio.setVolume(55.f);   // 0–100; ajuste ao gosto
        gerAudio.tocar();
    }

    //HUD
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
            "D=andar  S=parar  W=pular  D+E=cambalhota\n"
            "J=soco  J+K=combo  W+U=chute-ar  W+I=soco-ar\n"
            "J+K+Spc=ESPECIAL  G=segurar  G+T=tapa  G+R=arremesso\n"
            "Z=fatal  V=vencer  BkSp=reset"
        );
    }

    //Timer de pulo
    float timerPulo  = 0.f;
    bool  estaNoPulo = false;

    Estado estadoAnterior = automato.getEstado();

    sf::Clock relogio;

    // Lambda: envia entrada ao autômato e sincroniza o gerenciador de sprites
    auto enviar = [&](Entrada e) {
        automato.processar(e);
        Estado novo = automato.getEstado();
        if (novo != estadoAnterior) {
            gerSprites.trocarEstado(novo);
            estadoAnterior = novo;
        }
    };

    //Loop principal
    while (janela.isOpen()) {

        float dt = relogio.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        // Eventos
        sf::Event ev;
        while (janela.pollEvent(ev)) {

            if (ev.type == sf::Event::Closed)
                janela.close();

            if (ev.type == sf::Event::KeyPressed) {
                Estado est = automato.getEstado();
                bool emAr  = (est == Estado::PULANDO    ||
                              est == Estado::PULO_FRENTE ||
                              est == Estado::CHUTE_AEREO ||
                              est == Estado::SOCO_DESCIDA);

                switch (ev.key.code) {

                    case sf::Keyboard::D: enviar(Entrada::ANDAR); break;
                    case sf::Keyboard::S: enviar(Entrada::PARAR); break;

                    case sf::Keyboard::W:
                        enviar(Entrada::PULAR);
                        estaNoPulo = true; timerPulo = 0.f;
                        break;

                    case sf::Keyboard::E:
                        enviar(Entrada::PULAR_FRENTE);
                        estaNoPulo = true; timerPulo = 0.f;
                        break;

                    case sf::Keyboard::J:
                        if (emAr) enviar(Entrada::CHUTE_NO_AR);
                        else      enviar(Entrada::SOCO);
                        break;

                    case sf::Keyboard::K:  enviar(Entrada::SOCO_NOVAMENTE);  break;
                    case sf::Keyboard::U:  enviar(Entrada::CHUTE_NO_AR);     break;
                    case sf::Keyboard::I:  enviar(Entrada::SOCO_NO_AR_BAIXO);break;
                    case sf::Keyboard::Space: enviar(Entrada::ESPECIAL);     break;
                    case sf::Keyboard::G:  enviar(Entrada::SEGURAR_INIMIGO); break;
                    case sf::Keyboard::T:  enviar(Entrada::TAPA);            break;
                    case sf::Keyboard::R:  enviar(Entrada::ARREMESSAR);      break;
                    case sf::Keyboard::H:  enviar(Entrada::TOMAR_HIT);       break;
                    case sf::Keyboard::F:  enviar(Entrada::TOMAR_HIT_FORTE); break;
                    case sf::Keyboard::Z:  enviar(Entrada::TOMAR_HIT_FATAL); break;
                    case sf::Keyboard::V:  enviar(Entrada::VENCER);          break;

                    case sf::Keyboard::BackSpace:
                        automato.resetar();
                        gerSprites.trocarEstado(Estado::PARADO);
                        estadoAnterior = Estado::PARADO;
                        estaNoPulo     = false;
                        break;

                    default: break;
                }
            }
        }

        // Lógica: auto-aterrissar após DURACAO_PULO
        Estado estAtual = automato.getEstado();
        bool emAr = (estAtual == Estado::PULANDO    ||
                     estAtual == Estado::PULO_FRENTE ||
                     estAtual == Estado::CHUTE_AEREO ||
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
        if (animConcluida)
            enviar(Entrada::ANIMACAO_CONCLUIDA);

        // HUD
        if (fonteOk)
            textoEstado.setString("Estado: " + automato.getNomeEstado());

        //Render
        janela.clear();

        if (fundoComImagem)
            janela.draw(spriteFundo);
        else
            janela.draw(fundoFallback);

        janela.draw(sprite);

        if (fonteOk) {
            janela.draw(textoEstado);
            janela.draw(textoControles);
        }

        janela.display();
    }

    // sf::Music para automaticamente no destrutor de GerenciadorAudio
    return 0;
}