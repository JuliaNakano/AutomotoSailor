#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

class GerenciadorAudio {
public:
    GerenciadorAudio();

    bool carregar(const std::string& caminho);

    void tocar();
    void pausar();
    void parar();

    void setVolume(float volume);

    bool isRodando() const;

private:
    sf::Music musica;
};