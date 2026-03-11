#include <exception>
#include <iostream>
using namespace std;

/*  Contenu de la classe exception

class exception 
{
public:
    exception() noexcept; //Constructeur.
    virtual  exception() noexcept;; //Destructeur.
 
    virtual const char* what() const noexcept; //Renvoie une chaÃ®ne "Ã  la C" contenant des infos sur l'erreur.
};

*/
 
class Erreur: public exception {
public:
    Erreur(int numero=0, string const& phrase="", int niveau=0) throw();
 
    virtual const char* what() const noexcept { return m_phrase.c_str(); }
    
    int getNiveau() const noexcept;    
    virtual ~Erreur() noexcept {}

 
private:
    int m_numero;               //Numero de l'erreur
    string m_phrase;            //Description de l'erreur
    int m_niveau;               //Niveau de l'erreur
    /*
    0: info/minor
    1: warning
    2: serious error
    3: critical!
    */
};
