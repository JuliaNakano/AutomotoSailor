#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

// ============================================================
//  GerenciadorAudio
//
//  Encapsula a música de fundo do jogo (sf::Music).
//  sf::Music faz streaming do arquivo em disco, ao contrário
//  de sf::SoundBuffer, portanto é a escolha certa para faixas
//  longas — sem estouro de memória.
//
//  Uso típico em main.cpp:
//      GerenciadorAudio audio;
//      audio.carregar("assets/audio/musica.ogg");
//      audio.tocar();          // já com loop ativado
//
//  Formatos suportados pelo SFML: OGG/Vorbis, WAV, FLAC.
//  Prefira OGG: boa compressão e sem patentes.
// ============================================================

class GerenciadorAudio {
public:
    GerenciadorAudio();

    // Abre o arquivo de música e ativa loop.
    // Retorna false se o arquivo não for encontrado.
    bool carregar(const std::string& caminho);

    // Controle de reprodução
    void tocar();
    void pausar();
    void parar();

    // Volume de 0 (mudo) a 100 (máximo). Padrão: 50.
    void setVolume(float volume);

    bool isRodando() const;

private:
    sf::Music musica;
};