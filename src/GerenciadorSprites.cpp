#include "GerenciadorSprites.h"
#include <iostream>

// ============================================================
//  MAPA DE COORDENADAS DO SPRITESHEET  (535 × 1600 px)
//
//  Todas as posições foram detectadas automaticamente por
//  análise de pixels do fundo ciano (R≈127, G≈255, B≈254).
//
//  Se algum frame ficar desalinhado, ajuste os valores de
//  x, y, w, h abaixo diretamente neste arquivo.
//
//  Formato de cada quadro:  adicionarQuadro(x, y, w, h, duracao_s)
// ============================================================

// ── FAIXAS PRINCIPAIS (y, altura) ───────────────────────────
// Linha 1 – Stay / Punch / Go           y=10,  h=83
// Linha 2 – Kombo after punch           y=131, h=84
// Linha 3 – Jump kick / Punch down /    y=244, h=86
//           Spezial (pequeno)
// Linha 4 – Hold / Slap / Throw         y=387, h=72
// Linha 5 – Get hit / Hurt / Def / Win  y=531, h=78
// Special grandes (4 sub-linhas)
//   spec1                               y=660, h=92
//   spec2                               y=771, h=66
//   spec3                               y=869, h=82
//   spec4                               y=967, h=93
// Jump / Jump Forward                   y=1131, h=85
// ============================================================

GerenciadorSprites::GerenciadorSprites()
    : estadoAnterior(Estado::PARADO) {}

// Carregamento

bool GerenciadorSprites::carregar(const std::string& caminho) {
    if (!textura.loadFromFile(caminho)) {
        std::cerr << "[Sprites] ERRO: nao foi possivel carregar " << caminho << "\n";
        return false;
    }
    textura.setSmooth(false);
    inicializarAnimacoes();
    std::cout << "[Sprites] Spritesheet carregado: " << caminho
              << " (" << textura.getSize().x << "x" << textura.getSize().y << ")\n";
    return true;
}

// Helpers

AnimacaoSprite& GerenciadorSprites::criarAnimacao(Estado estado, bool loop) {
    animacoes[estado] = AnimacaoSprite();
    animacoes[estado].setLoop(loop);
    return animacoes[estado];
}

// Definição de todas as animações

void GerenciadorSprites::inicializarAnimacoes() {

    // ── PARADO (Stay) ── Linha 1, 4 quadros, loop ────────────
    {
        auto& a = criarAnimacao(Estado::PARADO, true);
        a.adicionarQuadro( 15, 10, 43, 83, 0.18f);  // f1
        a.adicionarQuadro( 58, 10, 36, 83, 0.18f);  // f2
        a.adicionarQuadro( 94, 10, 30, 83, 0.18f);  // f3
        a.adicionarQuadro(130, 10, 48, 83, 0.18f);  // f4 (estimado)
    }

    // ── ANDANDO (Go) ── Linha 1, 5 quadros, loop ─────────────
    {
        auto& a = criarAnimacao(Estado::ANDANDO, true);
        a.adicionarQuadro(327, 10, 48, 83, 0.10f);  // f1
        a.adicionarQuadro(375, 10, 50, 83, 0.10f);  // f2
        a.adicionarQuadro(425, 10, 51, 83, 0.10f);  // f3
        a.adicionarQuadro(476, 10, 49, 83, 0.10f);  // f4
    }

    // ── SOCO (Punch) ── Linha 1, 3 quadros, sem loop ─────────
    {
        auto& a = criarAnimacao(Estado::SOCO, false);
        a.adicionarQuadro(177, 10, 64, 83, 0.10f);  // f1 – preparando
        a.adicionarQuadro(241, 10, 42, 83, 0.10f);  // f2 – estendendo
        a.adicionarQuadro(283, 10, 44, 83, 0.12f);  // f3 – impacto
    }

    // ── COMBO (Kombo after punch) ── Linha 2, 6 quadros ──────
    {
        auto& a = criarAnimacao(Estado::COMBO, false);
        a.adicionarQuadro(  5, 131, 42, 84, 0.09f);  // f1
        a.adicionarQuadro( 47, 131, 54, 84, 0.09f);  // f2
        a.adicionarQuadro(101, 131, 53, 84, 0.09f);  // f3
        a.adicionarQuadro(165, 131, 41, 84, 0.09f);  // f4
        a.adicionarQuadro(206, 131, 51, 84, 0.09f);  // f5
        a.adicionarQuadro(257, 131, 50, 84, 0.12f);  // f6 – pose final
    }

    // ── SEGURAR (Hold) ── Linha 4, 2 quadros, loop ───────────
    {
        auto& a = criarAnimacao(Estado::SEGURAR, true);
        a.adicionarQuadro( 19, 387, 55, 72, 0.20f);  // f1
        a.adicionarQuadro( 74, 387, 60, 72, 0.20f);  // f2
    }

    // ── TAPA_SEGURADO (Slap with hold) ── Linha 4, 1 quadro largo
    {
        auto& a = criarAnimacao(Estado::TAPA_SEGURADO, false);
        a.adicionarQuadro(145, 387, 119, 72, 0.25f);  // 2 personagens
    }

    // ── ARREMESSO (Throw) ── Linha 4, 3 quadros, sem loop ────
    {
        auto& a = criarAnimacao(Estado::ARREMESSO, false);
        a.adicionarQuadro(264, 387,  63, 72, 0.10f);  // f1
        a.adicionarQuadro(327, 387,  70, 72, 0.10f);  // f2
        a.adicionarQuadro(397, 387, 138, 72, 0.20f);  // f3 – voo largo
    }

    // ── PULANDO (Jump vertical) ── Linha Jump, 3 quadros ─────
    {
        auto& a = criarAnimacao(Estado::PULANDO, false);
        a.adicionarQuadro( 21, 1131, 43, 85, 0.14f);  // f1 – subindo
        a.adicionarQuadro( 64, 1131, 54, 85, 0.22f);  // f2 – apex
        a.adicionarQuadro(118, 1131, 36, 85, 0.14f);  // f3 – descendo
    }

    // ── PULO_FRENTE (Jump forward) ── Linha Jump, 4 quadros ──
    {
        auto& a = criarAnimacao(Estado::PULO_FRENTE, false);
        a.adicionarQuadro(201, 1131,  70, 85, 0.12f);  // f1
        a.adicionarQuadro(271, 1131,  69, 85, 0.18f);  // f2 – planando
        a.adicionarQuadro(340, 1131,  74, 85, 0.12f);  // f3
        a.adicionarQuadro(414, 1131, 121, 85, 0.10f);  // f4 – wide pose
    }

    // ── CHUTE_AEREO (Jump and kick) ── Linha 3, 3 quadros ────
    {
        auto& a = criarAnimacao(Estado::CHUTE_AEREO, false);
        a.adicionarQuadro( 23, 244, 63, 86, 0.10f);  // f1 – preparando
        a.adicionarQuadro( 95, 244, 75, 86, 0.10f);  // f2 – chute
        a.adicionarQuadro(170, 244, 51, 86, 0.12f);  // f3 – recuperando
    }

    // ── SOCO_DESCIDA (Jump and punch down) ── Linha 3, 2 quadros
    {
        auto& a = criarAnimacao(Estado::SOCO_DESCIDA, false);
        a.adicionarQuadro(227, 244, 33, 86, 0.10f);  // f1
        a.adicionarQuadro(260, 244, 47, 86, 0.12f);  // f2
    }

    // ── ESPECIAL (Spezial grande, 4 faixas × ~9 quadros) ─────
    //    A animação percorre as 4 linhas em sequência: spec1 → spec4
    //    Total: ~36 quadros ≈ 3,6 s a 0.10 s/quadro
    {
        auto& a = criarAnimacao(Estado::ESPECIAL, false);

        // spec1  y=660, h=92
        a.adicionarQuadro( 53, 660,  50, 92, 0.09f);
        a.adicionarQuadro(103, 660,  61, 92, 0.09f);
        a.adicionarQuadro(164, 660,  53, 92, 0.09f);
        a.adicionarQuadro(217, 660,  61, 92, 0.09f);
        a.adicionarQuadro(278, 660,  61, 92, 0.09f);
        a.adicionarQuadro(339, 660,  31, 92, 0.08f);
        a.adicionarQuadro(370, 660,  34, 92, 0.08f);
        a.adicionarQuadro(404, 660,  30, 92, 0.08f);
        a.adicionarQuadro(434, 660,  40, 92, 0.08f);

        // spec2  y=771, h=66
        a.adicionarQuadro(  0, 771,  29, 66, 0.08f);
        a.adicionarQuadro( 29, 771,  30, 66, 0.08f);
        a.adicionarQuadro( 59, 771,  34, 66, 0.08f);
        a.adicionarQuadro( 93, 771,  30, 66, 0.08f);
        a.adicionarQuadro(123, 771,  38, 66, 0.08f);
        a.adicionarQuadro(161, 771,  29, 66, 0.08f);
        a.adicionarQuadro(190, 771,  37, 66, 0.08f);
        a.adicionarQuadro(227, 771,  31, 66, 0.08f);
        a.adicionarQuadro(258, 771,  34, 66, 0.08f);

        // spec3  y=869, h=82
        a.adicionarQuadro( 19, 869,  59, 82, 0.09f);
        a.adicionarQuadro( 78, 869,  58, 82, 0.09f);
        a.adicionarQuadro(136, 869,  57, 82, 0.09f);
        a.adicionarQuadro(193, 869,  54, 82, 0.09f);
        a.adicionarQuadro(247, 869,  55, 82, 0.09f);
        a.adicionarQuadro(302, 869,  53, 82, 0.09f);
        a.adicionarQuadro(355, 869,  52, 82, 0.09f);
        a.adicionarQuadro(407, 869,  54, 82, 0.09f);
        a.adicionarQuadro(461, 869,  74, 82, 0.09f);

        // spec4  y=967, h=93  (pose de retorno)
        a.adicionarQuadro( 22, 967,  49, 93, 0.09f);
        a.adicionarQuadro( 71, 967,  57, 93, 0.09f);
        a.adicionarQuadro(128, 967,  56, 93, 0.09f);
        a.adicionarQuadro(184, 967,  53, 93, 0.09f);
        a.adicionarQuadro(237, 967,  57, 93, 0.09f);
        a.adicionarQuadro(294, 967,  54, 93, 0.09f);
        a.adicionarQuadro(348, 967,  48, 93, 0.09f);
        a.adicionarQuadro(396, 967,  46, 93, 0.09f);
        a.adicionarQuadro(442, 967,  93, 93, 0.12f);  // pose final longa
    }

    // ── TOMANDO_HIT (Get hit) ── Linha 5, 1 quadro ───────────
    {
        auto& a = criarAnimacao(Estado::TOMANDO_HIT, false);
        a.adicionarQuadro(  0, 531, 48, 78, 0.15f);  // recua
        a.adicionarQuadro( 55, 531, 62, 78, 0.10f);  // para (reusa hurt f1)
    }

    // ── MACHUCADO (Hurt) ── Linha 5, 3 quadros ───────────────
    {
        auto& a = criarAnimacao(Estado::MACHUCADO, false);
        a.adicionarQuadro( 55, 531,  62, 78, 0.12f);  // f1 – tombando
        a.adicionarQuadro(117, 531,  82, 78, 0.12f);  // f2 – no chão
        a.adicionarQuadro(199, 531,  77, 78, 0.20f);  // f3 – finalizado
    }

    // ── DERROTADO (Defeated) ── Linha 5, 2 quadros, sem loop ─
    {
        auto& a = criarAnimacao(Estado::DERROTADO, false);
        a.adicionarQuadro(284, 531, 56, 78, 0.15f);  // f1
        a.adicionarQuadro(340, 531, 55, 78, 0.60f);  // f2 – congela
    }

    // ── VITORIA (Win) ── Linha 5, 2 quadros, loop ────────────
    {
        auto& a = criarAnimacao(Estado::VITORIA, true);
        a.adicionarQuadro(414, 531, 52, 78, 0.25f);  // f1
        a.adicionarQuadro(476, 531, 59, 78, 0.25f);  // f2
    }

    std::cout << "[Sprites] " << animacoes.size() << " animacoes carregadas.\n";
}

// Atualização / Troca de estado

void GerenciadorSprites::trocarEstado(Estado novoEstado) {
    if (novoEstado == estadoAnterior) return;

    // Reinicia a nova animação quando o estado muda
    auto it = animacoes.find(novoEstado);
    if (it != animacoes.end())
        it->second.reiniciar();

    estadoAnterior = novoEstado;
}

bool GerenciadorSprites::atualizar(Estado estado, float dt, sf::Sprite& sprite) {
    auto it = animacoes.find(estado);
    if (it == animacoes.end()) return false;

    AnimacaoSprite& anim = it->second;
    bool concluida = anim.atualizar(dt);
    anim.aplicar(sprite);

    return concluida;  // true → Jogo deve enviar ANIMACAO_CONCLUIDA ao autômato
}

const sf::Texture& GerenciadorSprites::getTextura() const {
    return textura;
}
