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
    sf::Image imagemTemporaria;
    
    if (!imagemTemporaria.loadFromFile(caminho)) {
        std::cerr << "[Sprites] ERRO: nao foi possivel carregar a imagem " << caminho << "\n";
        return false;
    }

    sf::Color corDeFundo = imagemTemporaria.getPixel(0, 0); 
    imagemTemporaria.createMaskFromColor(corDeFundo); 

    if (!textura.loadFromImage(imagemTemporaria)) {
        std::cerr << "[Sprites] ERRO: nao foi possivel passar a imagem para a textura " << caminho << "\n";
        return false;
    }

    textura.setSmooth(false);
    inicializarAnimacoes();
    std::cout << "[Sprites] Spritesheet carregado sem fundo: " << caminho
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
        a.adicionarQuadro( 64, 13, 44, 94, 0.18f);  // f1
        a.adicionarQuadro( 111, 12, 42, 95, 0.18f);  // f2
    }

    // ── ANDANDO (Go) ── Linha 1, 5 quadros, loop ─────────────
    {
        auto& a = criarAnimacao(Estado::ANDANDO, true);
        a.adicionarQuadro(308, 14, 37, 95, 0.10f);  // f1
        a.adicionarQuadro(362, 14, 37, 95, 0.10f);  // f2
        a.adicionarQuadro(418, 14, 37, 95, 0.10f);  // f3
        a.adicionarQuadro(479, 14, 37, 95, 0.10f);  // f4
        a.adicionarQuadro(543, 14, 37, 95, 0.10f);  // f5
        a.adicionarQuadro(608, 14, 37, 95, 0.10f);  // f6
    }

    // ── SOCO (Punch) ── Linha 1, 3 quadros, sem loop ─────────
    {
        auto& a = criarAnimacao(Estado::SOCO, false);
        a.adicionarQuadro(166, 13, 47, 94, 0.10f);  // f1 – preparando
        a.adicionarQuadro(227, 15, 64, 92, 0.10f);  // f2 – estendendo
        a.adicionarQuadro(166, 13, 47, 94, 0.12f);  // f3 – impacto
    }

    // ── COMBO (Kombo after punch) ── Linha 2, 6 quadros ──────
    {
        auto& a = criarAnimacao(Estado::COMBO, false);
        a.adicionarQuadro(  6, 174, 46, 92, 0.09f);  // f1
        a.adicionarQuadro( 60, 175, 62, 90, 0.09f);  // f2
        a.adicionarQuadro(129, 173, 61, 93, 0.09f);  // f3
        a.adicionarQuadro(194, 174, 59, 90, 0.09f);  // f4
        a.adicionarQuadro(262, 173, 49, 92, 0.09f);  // f5
        a.adicionarQuadro(327, 166, 52, 99, 0.09f);  // f6 – pose final
        a.adicionarQuadro(391, 171, 92, 94, 0.09f);  // f6 – pose final
        a.adicionarQuadro(484, 165, 92, 94, 0.09f);  // f6 – pose final
        a.adicionarQuadro(576, 178, 92, 100, 0.09f);  // f6 – pose final
    }

    // ── SEGURAR (Hold) ── Linha 4, 2 quadros, loop ───────────
    {
        auto& a = criarAnimacao(Estado::SEGURAR, true);
        a.adicionarQuadro( 25, 493, 51, 92, 0.20f);  // f1
    }

    // ── TAPA_SEGURADO (Slap with hold) ── Linha 4, 1 quadro largo
    {
        auto& a = criarAnimacao(Estado::TAPA_SEGURADO, true);
        a.adicionarQuadro(95, 494, 55, 93, 0.25f); // 2 personagens
        a.adicionarQuadro(172, 497, 58, 88, 0.25f);
    }

    // ── ARREMESSO (Throw) ── Linha 4, 3 quadros, sem loop ────
    {
        auto& a = criarAnimacao(Estado::ARREMESSO, false);
        a.adicionarQuadro(334, 496,  51, 90, 0.10f);  // f1
        a.adicionarQuadro(416, 494,  57, 93, 0.10f);  // f2
        a.adicionarQuadro(504, 484, 67, 103, 0.20f);  // f3 – voo largo
    }

    // ── PULANDO (Jump vertical) ── Linha Jump, 3 quadros ─────
    {
        auto& a = criarAnimacao(Estado::PULANDO, false); // f1 – subindo
        a.adicionarQuadro( 26, 1446, 42, 82, 0.10f);  // f2 – apex
        a.adicionarQuadro(79, 1433, 35, 96, 0.15f);  // f3 – descendo
        a.adicionarQuadro(138, 1435, 41, 63, 0.10f);  // f3 – descendo
    }

    // ── PULO_FRENTE (Jump forward) ── Linha Jump, 4 quadros ──
    {
        auto& a = criarAnimacao(Estado::PULO_FRENTE, false);
        a.adicionarQuadro(246, 1447,  48, 53, 0.12f);  // f1
        a.adicionarQuadro(340, 1443,  35, 57, 0.18f);  // f2 – planando
        a.adicionarQuadro(426, 1443,  51, 51, 0.12f);  // f3
        a.adicionarQuadro(528, 1442, 38, 59, 0.10f);  // f4 – wide pose
    }

    // ── CHUTE_AEREO (Jump and kick) ── Linha 3, 3 quadros ────
    {
        auto& a = criarAnimacao(Estado::CHUTE_AEREO, false);
        a.adicionarQuadro( 1, 314, 80, 87, 0.10f);  // f1 – preparando
        a.adicionarQuadro( 85, 318, 119, 78, 0.10f);  // f2 – chute
    }

    // ── SOCO_DESCIDA (Jump and punch down) ── Linha 3, 2 quadros
    {
        auto& a = criarAnimacao(Estado::SOCO_DESCIDA, false);
        a.adicionarQuadro(209, 330, 73, 65, 0.22f);  // f1
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
        a.adicionarQuadro( 3, 687, 45, 89, 0.10f);
    }

    // ── MACHUCADO (Hurt) ── Linha 5, 3 quadros ───────────────
    {
        auto& a = criarAnimacao(Estado::MACHUCADO, false);
        a.adicionarQuadro(  66, 692, 81, 62, 0.15f);  // recua
        a.adicionarQuadro( 149, 735, 88, 43, 0.10f);  // para (reusa hurt f1)
        a.adicionarQuadro( 241, 735, 70, 43, 0.10f);
        a.adicionarQuadro( 149, 735, 88, 43, 0.10f);
    }

    // ── DERROTADO (Defeated) ── Linha 5, 2 quadros, sem loop ─
    {
        auto& a = criarAnimacao(Estado::DERROTADO, false);
        a.adicionarQuadro(359, 720, 61, 57, 0.15f);  // f1
        a.adicionarQuadro(430, 719, 61, 57, 0.60f);  // f2 – congela
    }

    // ── VITORIA (Win) ── Linha 5, 2 quadros, loop ────────────
    {
        auto& a = criarAnimacao(Estado::VITORIA, true);
        a.adicionarQuadro(529, 677, 44, 95, 0.25f);  // f1
        a.adicionarQuadro(595, 674, 61, 96, 0.25f);  // f2
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
