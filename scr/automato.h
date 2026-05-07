#pragma once
#include <string>
#include <map>
#include <iostream>

// ============================================================
//  AFD — Automato
//  Estados e transições do autômato
// ============================================================

enum class Estado {
    // Neutros
    PARADO,
    ANDANDO,

    // Ataques terrestres
    SOCO,
    COMBO,
    SEGURAR,
    TAPA_SEGURADO,
    ARREMESSO,

    // Aéreos
    PULANDO,
    PULO_FRENTE,
    CHUTE_AEREO,
    SOCO_DESCIDA,

    // Especial
    ESPECIAL,

    // Dano
    TOMANDO_HIT,
    MACHUCADO,
    DERROTADO,

    // Vitória
    VITORIA
};

enum class Entrada {
    // Movimentação
    ANDAR,
    PARAR,
    PULAR,
    PULAR_FRENTE,

    // Ataques básicos
    SOCO,
    SOCO_NOVAMENTE,
    CHUTE_NO_AR,
    SOCO_NO_AR_BAIXO,

    // Ataques especiais
    ESPECIAL,
    SEGURAR_INIMIGO,
    TAPA,
    ARREMESSAR,

    // Eventos
    ATERRISSAR,
    TOMAR_HIT,
    TOMAR_HIT_FORTE,
    TOMAR_HIT_FATAL,

    // Controle de animações
    ANIMACAO_CONCLUIDA,
    VENCER
};

// ============================================================
//  Classe do AFD
// ============================================================
class Automato {
public:
    Automato();

    // Processa entrada e executa transição
    void processar(Entrada entrada);

    // Getters
    Estado      getEstado()      const;
    std::string getNomeEstado()  const;
    bool        isDerrotado()    const;
    bool        isVencedor()     const;

    // Utilitários
    static std::string getNomeEntrada(Entrada e);
    void resetar();

private:
    Estado estadoAtual;

    // Tabela de transições
    std::map<Estado, std::map<Entrada, Estado>> delta;

    void inicializarTransicoes();
    void executarAcao(Estado de, Estado para, Entrada entrada);

    static const std::map<Estado, std::string>& mapaEstados();
    static const std::map<Entrada, std::string>& mapaEntradas();
};