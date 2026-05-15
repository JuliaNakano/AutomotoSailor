#include "GerenciadorAudio.h"

GerenciadorAudio::GerenciadorAudio() {
    // Volume inicial confortável — pode ser ajustado em main.cpp
    musica.setVolume(50.f);
}

bool GerenciadorAudio::carregar(const std::string& caminho) {
    // sf::Music::openFromFile faz streaming; não carrega tudo na RAM
    if (!musica.openFromFile(caminho)) {
        std::cerr << "[Audio] ERRO: nao foi possivel abrir " << caminho << "\n"
                  << "        Formatos suportados: OGG, WAV, FLAC.\n";
        return false;
    }

    musica.setLoop(true);   // loop infinito — comportamento padrão de BGM

    std::cout << "[Audio] Musica carregada: " << caminho
              << "  (duracao: " << musica.getDuration().asSeconds() << " s)\n";
    return true;
}

void GerenciadorAudio::tocar()  { musica.play();  }
void GerenciadorAudio::pausar() { musica.pause(); }
void GerenciadorAudio::parar()  { musica.stop();  }

void GerenciadorAudio::setVolume(float volume) {
    // SFML aceita 0–100; clampamos por segurança
    if (volume < 0.f)   volume = 0.f;
    if (volume > 100.f) volume = 100.f;
    musica.setVolume(volume);
}

bool GerenciadorAudio::isRodando() const {
    return musica.getStatus() == sf::Music::Playing;
}