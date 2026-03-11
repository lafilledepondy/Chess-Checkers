#include "exception.hpp"

Erreur::Erreur(int numero, string const& phrase, int niveau) throw() {
    this->m_numero = numero;
    this->m_phrase = phrase;
    this->m_niveau = niveau;
}

int Erreur::getNiveau() const noexcept{
    return this->m_niveau;
}