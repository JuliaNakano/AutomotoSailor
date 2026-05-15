#include "automato.h"

//  Mapas estáticos de nomes (para log/debug)

const std::map<Estado, std::string>& Automato::mapaEstados() {
    static const std::map<Estado, std::string> m = {
        {Estado::PARADO,        "PARADO"},
        {Estado::ANDANDO,       "ANDANDO"},
        {Estado::SOCO,          "SOCO"},
        {Estado::COMBO,         "COMBO"},
        {Estado::SEGURAR,       "SEGURAR"},
        {Estado::TAPA_SEGURADO, "TAPA_SEGURADO"},
        {Estado::ARREMESSO,     "ARREMESSO"},
        {Estado::PULANDO,       "PULANDO"},
        {Estado::PULO_FRENTE,   "PULO_FRENTE"},
        {Estado::CHUTE_AEREO,   "CHUTE_AEREO"},
        {Estado::SOCO_DESCIDA,  "SOCO_DESCIDA"},
        {Estado::ESPECIAL,      "ESPECIAL"},
        {Estado::TOMANDO_HIT,   "TOMANDO_HIT"},
        {Estado::MACHUCADO,     "MACHUCADO"},
        {Estado::DERROTADO,     "DERROTADO"},
        {Estado::VITORIA,       "VITORIA"}
    };
    return m;
}

const std::map<Entrada, std::string>& Automato::mapaEntradas() {
    static const std::map<Entrada, std::string> m = {
        {Entrada::ANDAR,               "ANDAR"},
        {Entrada::PARAR,               "PARAR"},
        {Entrada::PULAR,               "PULAR"},
        {Entrada::PULAR_FRENTE,        "PULAR_FRENTE"},
        {Entrada::SOCO,                "SOCO"},
        {Entrada::SOCO_NOVAMENTE,      "SOCO_NOVAMENTE"},
        {Entrada::CHUTE_NO_AR,         "CHUTE_NO_AR"},
        {Entrada::SOCO_NO_AR_BAIXO,    "SOCO_NO_AR_BAIXO"},
        {Entrada::ESPECIAL,            "ESPECIAL"},
        {Entrada::SEGURAR_INIMIGO,     "SEGURAR_INIMIGO"},
        {Entrada::TAPA,                "TAPA"},
        {Entrada::ARREMESSAR,          "ARREMESSAR"},
        {Entrada::ATERRISSAR,          "ATERRISSAR"},
        {Entrada::TOMAR_HIT,           "TOMAR_HIT"},
        {Entrada::TOMAR_HIT_FORTE,     "TOMAR_HIT_FORTE"},
        {Entrada::TOMAR_HIT_FATAL,     "TOMAR_HIT_FATAL"},
        {Entrada::ANIMACAO_CONCLUIDA,  "ANIMACAO_CONCLUIDA"},
        {Entrada::VENCER,              "VENCER"}
    };
    return m;
}

//  Construtor
Automato::Automato() : estadoAtual(Estado::PARADO) {
    inicializarTransicoes();
    std::cout << "[AFD] Automato iniciado. Estado: PARADO\n";
}

//  Tabela de transições (delta)
void Automato::inicializarTransicoes() {

    // ── PARADO ──────────────────────────────────────────────
    delta[Estado::PARADO][Entrada::ANDAR]           = Estado::ANDANDO;
    delta[Estado::PARADO][Entrada::PULAR]           = Estado::PULANDO;
    delta[Estado::PARADO][Entrada::PULAR_FRENTE]    = Estado::PULO_FRENTE;
    delta[Estado::PARADO][Entrada::SOCO]            = Estado::SOCO;
    delta[Estado::PARADO][Entrada::SEGURAR_INIMIGO] = Estado::SEGURAR;
    delta[Estado::PARADO][Entrada::TOMAR_HIT_FATAL] = Estado::DERROTADO;
    delta[Estado::PARADO][Entrada::VENCER]          = Estado::VITORIA;

    // ── ANDANDO ─────────────────────────────────────────────
    delta[Estado::ANDANDO][Entrada::PARAR]           = Estado::PARADO;
    delta[Estado::ANDANDO][Entrada::PULAR]           = Estado::PULANDO;
    delta[Estado::ANDANDO][Entrada::PULAR_FRENTE]    = Estado::PULO_FRENTE;
    delta[Estado::ANDANDO][Entrada::SOCO]            = Estado::SOCO;
    delta[Estado::ANDANDO][Entrada::SEGURAR_INIMIGO] = Estado::SEGURAR;
    delta[Estado::ANDANDO][Entrada::TOMAR_HIT_FATAL] = Estado::DERROTADO;
    delta[Estado::ANDANDO][Entrada::VENCER]          = Estado::VITORIA;

    // ── SOCO ────────────────────────────────────────────────
    delta[Estado::SOCO][Entrada::SOCO_NOVAMENTE]     = Estado::COMBO;
    delta[Estado::SOCO][Entrada::ANIMACAO_CONCLUIDA] = Estado::PARADO;
    delta[Estado::SOCO][Entrada::TOMAR_HIT_FATAL]    = Estado::DERROTADO;
    delta[Estado::SOCO][Entrada::VENCER]             = Estado::VITORIA;

    // ── COMBO ───────────────────────────────────────────────
    delta[Estado::COMBO][Entrada::ANIMACAO_CONCLUIDA] = Estado::PARADO;
    delta[Estado::COMBO][Entrada::TOMAR_HIT_FATAL]    = Estado::DERROTADO;
    delta[Estado::COMBO][Entrada::VENCER]             = Estado::VITORIA;
    delta[Estado::COMBO][Entrada::ESPECIAL]           = Estado::ESPECIAL;

    // ── SEGURAR ─────────────────────────────────────────────
    delta[Estado::SEGURAR][Entrada::TAPA]               = Estado::TAPA_SEGURADO;
    delta[Estado::SEGURAR][Entrada::ARREMESSAR]         = Estado::ARREMESSO;
    delta[Estado::SEGURAR][Entrada::ANIMACAO_CONCLUIDA] = Estado::PARADO;
    delta[Estado::SEGURAR][Entrada::TOMAR_HIT_FATAL]    = Estado::DERROTADO;

    // ── TAPA SEGURADO ──────────────────────────────────────
    delta[Estado::TAPA_SEGURADO][Entrada::ARREMESSAR]         = Estado::ARREMESSO;
    delta[Estado::TAPA_SEGURADO][Entrada::ANIMACAO_CONCLUIDA] = Estado::SEGURAR;
    delta[Estado::TAPA_SEGURADO][Entrada::TOMAR_HIT_FATAL]    = Estado::DERROTADO;

    // ── ARREMESSO ───────────────────────────────────────────
    delta[Estado::ARREMESSO][Entrada::ANIMACAO_CONCLUIDA] = Estado::PARADO;
    delta[Estado::ARREMESSO][Entrada::TOMAR_HIT_FATAL]    = Estado::DERROTADO;
    delta[Estado::ARREMESSO][Entrada::VENCER]              = Estado::VITORIA;

    // ── PULANDO ─────────────────────────────────────────────
    delta[Estado::PULANDO][Entrada::CHUTE_NO_AR]      = Estado::CHUTE_AEREO;
    delta[Estado::PULANDO][Entrada::SOCO_NO_AR_BAIXO] = Estado::SOCO_DESCIDA;
    delta[Estado::PULANDO][Entrada::ATERRISSAR]       = Estado::PARADO;
    delta[Estado::PULANDO][Entrada::TOMAR_HIT_FATAL]  = Estado::DERROTADO;

    // ── PULO FRENTE ─────────────────────────────────────────
    delta[Estado::PULO_FRENTE][Entrada::CHUTE_NO_AR]      = Estado::CHUTE_AEREO;
    delta[Estado::PULO_FRENTE][Entrada::SOCO_NO_AR_BAIXO] = Estado::SOCO_DESCIDA;
    delta[Estado::PULO_FRENTE][Entrada::ATERRISSAR]       = Estado::ANDANDO;
    delta[Estado::PULO_FRENTE][Entrada::TOMAR_HIT_FATAL]  = Estado::DERROTADO;

    // ── CHUTE AÉREO ─────────────────────────────────────────
    delta[Estado::CHUTE_AEREO][Entrada::ATERRISSAR]         = Estado::PARADO;
    delta[Estado::CHUTE_AEREO][Entrada::ANIMACAO_CONCLUIDA] = Estado::PULANDO;
    delta[Estado::CHUTE_AEREO][Entrada::TOMAR_HIT_FATAL]    = Estado::DERROTADO;

    // ── SOCO DESCIDA ────────────────────────────────────────
    delta[Estado::SOCO_DESCIDA][Entrada::ATERRISSAR]         = Estado::PARADO;
    delta[Estado::SOCO_DESCIDA][Entrada::ANIMACAO_CONCLUIDA] = Estado::PULANDO;
    delta[Estado::SOCO_DESCIDA][Entrada::TOMAR_HIT_FATAL]    = Estado::DERROTADO;

    // ── ESPECIAL ────────────────────────────────────────────
    delta[Estado::ESPECIAL][Entrada::ANIMACAO_CONCLUIDA] = Estado::PARADO;
    delta[Estado::ESPECIAL][Entrada::ATERRISSAR]         = Estado::PARADO;
    delta[Estado::ESPECIAL][Entrada::TOMAR_HIT_FATAL]    = Estado::DERROTADO;
    delta[Estado::ESPECIAL][Entrada::VENCER]             = Estado::VITORIA;

    // ── MACHUCADO ───────────────────────────────────────────
    delta[Estado::MACHUCADO][Entrada::ANIMACAO_CONCLUIDA] = Estado::PARADO;
    delta[Estado::MACHUCADO][Entrada::TOMAR_HIT_FATAL]    = Estado::DERROTADO;
}

void Automato::processar(Entrada entrada) {
    // Busca o estado atual na tabela delta
    auto itEstado = delta.find(estadoAtual);
    if (itEstado == delta.end()) {
        std::cout << "[AFD] Estado sem transições: " << getNomeEstado() << "\n";
        return;
    }

    // Busca a entrada dentro das transições do estado atual
    auto itEntrada = itEstado->second.find(entrada);
    if (itEntrada == itEstado->second.end()) {
        return;
    }

    Estado novoEstado = itEntrada->second;
    executarAcao(estadoAtual, novoEstado, entrada);
    estadoAtual = novoEstado;
}

// getters

Estado Automato::getEstado() const {
    return estadoAtual;
}

std::string Automato::getNomeEstado() const {
    const auto& m = mapaEstados();
    auto it = m.find(estadoAtual);
    return (it != m.end()) ? it->second : "DESCONHECIDO";
}

bool Automato::isDerrotado() const { return estadoAtual == Estado::DERROTADO; }
bool Automato::isVencedor()  const { return estadoAtual == Estado::VITORIA;   }

std::string Automato::getNomeEntrada(Entrada e) {
    const auto& m = mapaEntradas();
    auto it = m.find(e);
    return (it != m.end()) ? it->second : "DESCONHECIDA";
}

// resetar

void Automato::resetar() {
    estadoAtual = Estado::PARADO;
    std::cout << "[AFD] Automato resetado para PARADO.\n";
}


void Automato::executarAcao(Estado de, Estado para, Entrada entrada) {
    const auto& me = mapaEstados();
    std::cout << "[AFD] " << me.at(de)
              << " --[" << getNomeEntrada(entrada) << "]--> "
              << me.at(para) << "\n";
}