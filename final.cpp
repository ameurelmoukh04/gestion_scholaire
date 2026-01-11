#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <map>


using namespace std;
#define RESET   "\033[0m"
#define ROUGE   "\033[31m"
#define VERT    "\033[32m"
#define JAUNE   "\033[33m"
#define BLEU    "\033[34m"
#define CYAN    "\033[36m"
#define GRAS    "\033[1m"
int lireEntier();
float lireFloat();
bool verifierChoix(int choix, int min, int max);
string centrer(const string& texte, int largeur = 60);
void ligne();
void dessinerLigneHaut();
void dessinerLigneBas();
void dessinerLigneSep();
string encadrerTexte(const string& texte);
void afficherTitreSection(const string& titre);



// Forward declarations
class Etudiant;
class Enseignant;
class Caissiere;
class DirecteurPedagogique;

class Personne {
protected:
    int id;
    string prenom;
    string nom;
    string dateNaissance;
    string sexe;
    string email;
    string password;
    string telephone;
    string adresse;
    bool actif;
    int absence;
    int retard;

public:
    Personne()
        : id(0), actif(true), absence(0), retard(0) {}

    Personne(
        int id,
        const string& prenom,
        const string& nom,
        const string& dateNaissance,
        const string& email
    )
        : id(id),
          prenom(prenom),
          nom(nom),
          dateNaissance(dateNaissance),
          email(email),
          actif(true),
          absence(0),
          retard(0) {}

    virtual ~Personne() {}

    // Getters
    int getId() const { return id; }
    string getPrenom() const { return prenom; }
    string getNom() const { return nom; }
    string getNomComplet() const { return prenom + " " + nom; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    string getDateNaissance() const { return dateNaissance; }
    string getTelephone() const { return telephone; }
    string getAdresse() const { return adresse; }
    bool estActif() const { return actif; }
    int getAbsence() const { return absence; }
    int getRetard() const { return retard; }

    // Setters
    void setEmail(const string& email) { this->email = email; }
    void setPassword(const string& password) { this->password = password; }
    void setTelephone(const string& telephone) { this->telephone = telephone; }
    void setAdresse(const string& adresse) { this->adresse = adresse; }
    void setActif(bool statut) { actif = statut; }
    void setAbsence(int a) { absence = a; }
    void setRetard(int r) { retard = r; }

    // Absence / Retard
    void ajouterAbsence() { absence++; }
    void supprimerAbsence() { if (absence > 0) absence--; }
    void ajouterRetard() { retard++; }
    void supprimerRetard() { if (retard > 0) retard--; }

    // Affichage
    virtual     void afficherInfos() const {
        afficherTitreSection("MES INFORMATIONS");
        cout << centrer("ID : " + to_string(id)) << "\n";
        cout << centrer("Nom complet : " + getNomComplet()) << "\n";
        cout << centrer("Date de naissance : " + dateNaissance) << "\n";
        cout << centrer("Email : " + email) << "\n";
        cout << centrer("Telephone : " + telephone) << "\n";
        cout << centrer("Adresse : " + adresse) << "\n";
        cout << centrer("Absences : " + to_string(absence)) << "\n";
        cout << centrer("Retards : " + to_string(retard)) << "\n";
        cout << centrer("Actif : " + string(actif ? "Oui" : "Non")) << "\n";
    }

    // File I/O
    virtual string getType() const = 0;
    virtual string toFileString() const {
        stringstream ss;
        ss << id << ";" << prenom << ";" << nom << ";" << dateNaissance
           << ";" << email << ";" << password << ";" << telephone << ";" << adresse
           << ";" << actif << ";" << absence << ";" << retard;
        return ss.str();
    }

    virtual void fromFileString(const string& line) {
        // Ignorer les lignes de conflit Git
        if (line.find("<<<<<<<") != string::npos || 
            line.find("=======") != string::npos || 
            line.find(">>>>>>>") != string::npos) {
            return;
        }
        
        stringstream ss(line);
        string item;
        
        // Lire id
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    id = stoi(item);
                }
            } catch (...) {}
        }
        
        getline(ss, prenom, ';');
        getline(ss, nom, ';');
        getline(ss, dateNaissance, ';');
        getline(ss, email, ';');
        
        // Format du fichier : id;prenom;nom;date;email;password;telephone;adresse;actif;absence;retard
        // Lire le mot de passe (toujours présent dans le nouveau format)
        string nextField;
        if (getline(ss, nextField, ';')) {
            password = nextField;
            // Lire le téléphone
            if (getline(ss, telephone, ';')) {
                // Téléphone lu
            } else {
                telephone = "";
            }
        }
        
        if (getline(ss, adresse, ';')) {
            // Adresse lue
        } else {
            adresse = "";
        }
        
        if (getline(ss, item, ';')) {
            if (!item.empty()) actif = (item == "1" || item == "true");
        }
        
        if (getline(ss, item, ';')) {
            if (!item.empty()) {
                try {
                    if (item.find_first_not_of("0123456789") == string::npos) {
                        absence = stoi(item);
                    }
                } catch (...) {}
            }
        }
        
        if (getline(ss, item, ';')) {
            if (!item.empty()) {
                try {
                    if (item.find_first_not_of("0123456789") == string::npos) {
                        retard = stoi(item);
                    }
                } catch (...) {}
            }
        }
    }

    virtual void menu() = 0;
};

class Matiere {
    int id;
    string nom;
    int coefficient;
    string description;

public:
    Matiere(int i=0, string n="", int c=1, string d="")
        : id(i), nom(n), coefficient(c), description(d) {}

    int getId() const { return id; }
    string getNom() const { return nom; }
    int getCoefficient() const { return coefficient; }
    string getDescription() const { return description; }

    void modifier(string n, int c, string d) {
        nom = n;
        coefficient = c;
        description = d;
    }

    void afficher() const {
        cout << "Matiere [" << id << "] "
             << nom << " (coef " << coefficient << ") - "
             << description << endl;
    }

    string toFileString() const {
        stringstream ss;
        ss << id << ";" << nom << ";" << coefficient << ";" << description;
        return ss.str();
    }

    void fromFileString(const string& line) {
        // Ignorer les lignes de conflit Git
        if (line.find("<<<<<<<") != string::npos || 
            line.find("=======") != string::npos || 
            line.find(">>>>>>>") != string::npos) {
            return;
        }
        
        stringstream ss(line);
        string item;
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    id = stoi(item);
                }
            } catch (...) {}
        }
        
        getline(ss, nom, ';');
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    coefficient = stoi(item);
                }
            } catch (...) {}
        }
        
        getline(ss, description, ';');
    }

    void sauvegarder() const {
        ofstream f("matieres.txt", ios::app);
        f << toFileString() << "\n";
        f.close();
    }
};

class Note {
    int id;
    float valeur;
    string date;
    int idMatiere;

public:
    Note(int i=0, float v=0, string d="", int idM=0)
        : id(i), valeur(v), date(d), idMatiere(idM) {}

    int getId() const { return id; }
    float getValeur() const { return valeur; }
    string getDate() const { return date; }
    int getIdMatiere() const { return idMatiere; }

    void modifier(float v) { valeur = v; }
    void setDate(const string& d) { date = d; }
    void setIdMatiere(int idM) { idMatiere = idM; }

    void afficher() const {
        cout << "Note [" << id << "] "
             << valeur << " | Date: " << date
             << " | Matiere ID: " << idMatiere << endl;
    }

    string toFileString(int idEtudiant) const {
        stringstream ss;
        ss << id << ";" << idEtudiant << ";" << idMatiere << ";"
           << valeur << ";" << date;
        return ss.str();
    }

    void fromFileString(const string& line, int& idEtudiant) {
        // Ignorer les lignes de conflit Git
        if (line.find("<<<<<<<") != string::npos || 
            line.find("=======") != string::npos || 
            line.find(">>>>>>>") != string::npos) {
            return;
        }
        
        stringstream ss(line);
        string item;
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    id = stoi(item);
                }
            } catch (...) {}
        }
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    idEtudiant = stoi(item);
                }
            } catch (...) {}
        }
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    idMatiere = stoi(item);
                }
            } catch (...) {}
        }
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                valeur = stof(item);
            } catch (...) {}
        }
        
        getline(ss, date, ';');
    }

    void sauvegarder(int idEtudiant) const {
        ofstream f("notes.txt", ios::app);
        f << toFileString(idEtudiant) << "\n";
        f.close();
    }
};

class Salle {
    int id;
    string numero;
    int capacite;
    int etage;
    bool disponible;

public:
    Salle(int i=0, string n="", int c=0, int e=0)
        : id(i), numero(n), capacite(c), etage(e), disponible(true) {}

    int getId() const { return id; }
    string getNumero() const { return numero; }
    int getCapacite() const { return capacite; }
    int getEtage() const { return etage; }
    bool estDisponible() const { return disponible; }

    void reserver() { disponible = false; }
    void liberer() { disponible = true; }
    void modifier(string n, int c, int e) {
        numero = n;
        capacite = c;
        etage = e;
    }

    void afficher() const {
        cout << "Salle [" << id << "] "
             << numero << " | Cap: " << capacite
             << " | Etage: " << etage
             << " | Etat: "
             << (disponible ? "Libre" : "Occupee") << endl;
    }

    string toFileString() const {
        stringstream ss;
        ss << id << ";" << numero << ";" << capacite << ";"
           << etage << ";" << disponible;
        return ss.str();
    }

    void fromFileString(const string& line) {
        // Ignorer les lignes de conflit Git
        if (line.find("<<<<<<<") != string::npos || 
            line.find("=======") != string::npos || 
            line.find(">>>>>>>") != string::npos) {
            return;
        }
        
        stringstream ss(line);
        string item;
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    id = stoi(item);
                }
            } catch (...) {}
        }
        
        getline(ss, numero, ';');
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    capacite = stoi(item);
                }
            } catch (...) {}
        }
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    etage = stoi(item);
                }
            } catch (...) {}
        }
        
        if (getline(ss, item, ';')) {
            disponible = (item == "1" || item == "true");
        }
    }

    void sauvegarder() const {
        ofstream f("salles.txt", ios::app);
        f << toFileString() << "\n";
        f.close();
    }
};

class Groupe {
    int id;
    string nom;
    vector<int> idEtudiants;

public:
    Groupe(int i=0, string n="") : id(i), nom(n) {}

    int getId() const { return id; }
    string getNom() const { return nom; }
    vector<int>& getIdEtudiants() { return idEtudiants; }
    const vector<int>& getIdEtudiants() const { return idEtudiants; }

    void ajouterEtudiant(int idE) {
        idEtudiants.push_back(idE);
    }

    void supprimerEtudiant(int idE) {
        idEtudiants.erase(
            remove(idEtudiants.begin(), idEtudiants.end(), idE),
            idEtudiants.end()
        );
    }

    void afficher() const {
        cout << centrer("Groupe [" + to_string(id) + "] " + nom) << "\n";
        cout << centrer("Nombre d'etudiants : " + to_string(idEtudiants.size())) << "\n";
    }

    string toFileString() const {
        stringstream ss;
        ss << id << ";" << nom << ";";
        for (size_t i = 0; i < idEtudiants.size(); i++) {
            if (i > 0) ss << ",";
            ss << idEtudiants[i];
        }
        return ss.str();
    }

    void fromFileString(const string& line) {
        // Ignorer les lignes de conflit Git
        if (line.find("<<<<<<<") != string::npos || 
            line.find("=======") != string::npos || 
            line.find(">>>>>>>") != string::npos) {
            return;
        }
        
        stringstream ss(line);
        string item;
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    id = stoi(item);
                }
            } catch (...) {}
        }
        
        getline(ss, nom, ';');
        
        if (ss.peek() != EOF) {
            string etudiants;
            getline(ss, etudiants, ';');
            if (!etudiants.empty()) {
                stringstream ss2(etudiants);
                string idE;
                while (getline(ss2, idE, ',')) {
                    if (!idE.empty()) {
                        try {
                            if (idE.find_first_not_of("0123456789") == string::npos) {
                                idEtudiants.push_back(stoi(idE));
                            }
                        } catch (...) {}
                    }
                }
            }
        }
    }

    void sauvegarder() const {
        ofstream f("groupes.txt", ios::app);
        f << toFileString() << "\n";
        f.close();
    }
};

class Cours {
    int id;
    int idMatiere;
    int idSalle;
    string jour;
    string heureDebut;
    string heureFin;
    int idEnseignant;
    int idGroupe; // 0 si pas de groupe

public:
    Cours(int i=0, int idM=0, int idS=0, string j="", string hd="", string hf="", int idEns=0, int idG=0)
        : id(i), idMatiere(idM), idSalle(idS), jour(j), heureDebut(hd), heureFin(hf), idEnseignant(idEns), idGroupe(idG) {}

    int getId() const { return id; }
    int getIdMatiere() const { return idMatiere; }
    int getIdSalle() const { return idSalle; }
    string getJour() const { return jour; }
    string getHeureDebut() const { return heureDebut; }
    string getHeureFin() const { return heureFin; }
    int getIdEnseignant() const { return idEnseignant; }
    int getIdGroupe() const { return idGroupe; }

    void modifierHoraire(string hd, string hf) {
        heureDebut = hd;
        heureFin = hf;
    }

    void afficher() const {
        stringstream ss;
        ss << "Cours [" << id << "] " << jour << " " << heureDebut << "-" << heureFin
           << " | Matiere ID: " << idMatiere << " | Salle ID: " << idSalle
           << " | Enseignant ID: " << idEnseignant;
        if (idGroupe > 0) {
            ss << " | Groupe ID: " << idGroupe;
        }
        cout << centrer(ss.str()) << "\n";
    }

    string toFileString() const {
        stringstream ss;
        ss << id << ";" << idMatiere << ";" << idSalle << ";"
           << jour << ";" << heureDebut << ";" << heureFin << ";" << idEnseignant << ";" << idGroupe;
        return ss.str();
    }

    void fromFileString(const string& line) {
        // Ignorer les lignes de conflit Git
        if (line.find("<<<<<<<") != string::npos || 
            line.find("=======") != string::npos || 
            line.find(">>>>>>>") != string::npos) {
            return;
        }
        
        stringstream ss(line);
        string item;
        
        // Lire id
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    id = stoi(item);
                }
            } catch (...) {}
        }
        
        // Lire idMatiere
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    idMatiere = stoi(item);
                }
            } catch (...) {}
        }
        
        // Lire idSalle
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    idSalle = stoi(item);
                }
            } catch (...) {}
        }
        
        getline(ss, jour, ';');
        getline(ss, heureDebut, ';');
        getline(ss, heureFin, ';');
        
        // Gérer l'ancien format (avec idEtudiant) et le nouveau format (avec idEnseignant et idGroupe)
        idEnseignant = 0;
        idGroupe = 0;
        
        if (ss.peek() != EOF) {
            if (getline(ss, item, ';') && !item.empty()) {
                try {
                    // Vérifier si c'est un nombre
                    if (item.find_first_not_of("0123456789") == string::npos) {
                        int testId = stoi(item);
                        // Si le champ suivant existe, c'est le nouveau format (idEnseignant puis idGroupe)
                        if (ss.peek() != EOF) {
                            idEnseignant = testId;
                            if (getline(ss, item, ';') && !item.empty() && 
                                item.find_first_not_of("0123456789") == string::npos) {
                                idGroupe = stoi(item);
                            }
                        }
                        // Sinon, c'est l'ancien format avec idEtudiant, on l'ignore
                    }
                } catch (...) {
                    // Ignorer les erreurs de conversion
                }
            }
        }
    }

    void sauvegarder() const {
        ofstream f("cours.txt", ios::app);
        f << toFileString() << "\n";
        f.close();
    }
};

class Emploi {
    int idEtudiant;
    vector<Cours> cours;

public:
    Emploi(int idE=0) : idEtudiant(idE) {}

    void ajouterCours(const Cours& c) {
        cours.push_back(c);
    }

    void supprimerCours(int index) {
        if (index >= 0 && index < (int)cours.size())
            cours.erase(cours.begin() + index);
    }

    vector<Cours>& getCours() { return cours; }
    const vector<Cours>& getCours() const { return cours; }

    void consulter() const {
        afficherTitreSection("EMPLOI DU TEMPS");
        if (cours.empty()) {
            cout << centrer("Aucun cours planifie.") << "\n";
            return;
        }
        for (const auto& c : cours) {
            stringstream ss;
            ss << "Cours [" << c.getId() << "] " << c.getJour() << " " 
               << c.getHeureDebut() << "-" << c.getHeureFin()
               << " | Matiere ID: " << c.getIdMatiere()
               << " | Salle ID: " << c.getIdSalle();
            cout << centrer(ss.str()) << "\n";
        }
    }
};

class Paiement {
    int id;
    int idEtudiant;
    float montant;
    string date;
    string type;

public:
    Paiement(int i=0, int idE=0, float m=0, string d="", string t="")
        : id(i), idEtudiant(idE), montant(m), date(d), type(t) {}

    int getId() const { return id; }
    int getIdEtudiant() const { return idEtudiant; }
    float getMontant() const { return montant; }
    string getDate() const { return date; }
    string getType() const { return type; }

    void afficher() const {
        cout << "Paiement [" << id << "] "
             << montant << " DH | "
             << date << " | Type: " << type << endl;
    }

    string toFileString() const {
        stringstream ss;
        ss << id << ";" << idEtudiant << ";" << montant << ";" << date << ";" << type;
        return ss.str();
    }

    void fromFileString(const string& line) {
        // Ignorer les lignes de conflit Git
        if (line.find("<<<<<<<") != string::npos || 
            line.find("=======") != string::npos || 
            line.find(">>>>>>>") != string::npos) {
            return;
        }
        
        stringstream ss(line);
        string item;
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    id = stoi(item);
                }
            } catch (...) {}
        }
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                if (item.find_first_not_of("0123456789") == string::npos) {
                    idEtudiant = stoi(item);
                }
            } catch (...) {}
        }
        
        if (getline(ss, item, ';') && !item.empty()) {
            try {
                montant = stof(item);
            } catch (...) {}
        }
        
        getline(ss, date, ';');
        getline(ss, type, ';');
    }

    void sauvegarder() const {
        ofstream f("paiements.txt", ios::app);
        f << toFileString() << "\n";
        f.close();
    }
};

// Data Manager Class
class DataManager {
public:
    static vector<Personne*> personnes;
    static vector<Matiere> matieres;
    static vector<Salle> salles;
    static vector<Cours> cours;
    static vector<Etudiant*> etudiants;
    static vector<Note> notes;
    static vector<Paiement> paiements;
    static vector<Groupe> groupes;
    static int nextPersonneId;
    static int nextMatiereId;
    static int nextSalleId;
    static int nextCoursId;
    static int nextNoteId;
    static int nextPaiementId;
    static int nextGroupeId;
    static map<int, int> tentativesEchouees; // ID -> nombre de tentatives échouées

    static void chargerToutesLesDonnees() {
        chargerMatieres();
        chargerSalles();
        chargerGroupes();
        chargerPersonnes();
        chargerCours();
        chargerNotes();
        chargerPaiements();
    }

    static void sauvegarderToutesLesDonnees() {
        sauvegarderMatieres();
        sauvegarderSalles();
        sauvegarderGroupes();
        sauvegarderCours();
        sauvegarderPersonnes();
        sauvegarderNotes();
        sauvegarderPaiements();
    }

    static void chargerMatieres() {
        ifstream f("matieres.txt");
        if (!f.is_open()) return;
        matieres.clear();
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            Matiere m;
            m.fromFileString(line);
            matieres.push_back(m);
            if (m.getId() >= nextMatiereId) nextMatiereId = m.getId() + 1;
        }
        f.close();
    }

    static void sauvegarderMatieres() {
        ofstream f("matieres.txt");
        for (const auto& m : matieres)
            f << m.toFileString() << "\n";
        f.close();
    }

    static void chargerSalles() {
        ifstream f("salles.txt");
        if (!f.is_open()) return;
        salles.clear();
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            Salle s;
            s.fromFileString(line);
            salles.push_back(s);
            if (s.getId() >= nextSalleId) nextSalleId = s.getId() + 1;
        }
        f.close();
    }

    static void sauvegarderSalles() {
        ofstream f("salles.txt");
        for (const auto& s : salles)
            f << s.toFileString() << "\n";
        f.close();
    }

    static void chargerCours() {
        ifstream f("cours.txt");
        if (!f.is_open()) return;
        cours.clear();
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            Cours c;
            c.fromFileString(line);
            cours.push_back(c);
            if (c.getId() >= nextCoursId) nextCoursId = c.getId() + 1;
        }
        f.close();
    }

    static void sauvegarderCours() {
        ofstream f("cours.txt");
        for (const auto& c : cours)
            f << c.toFileString() << "\n";
        f.close();
    }

    static void chargerPersonnes(); // Implementation after class definitions

    static void sauvegarderPersonnes() {
        ofstream f("personnes.txt");
        for (auto* p : personnes) {
            f << p->getType() << ";" << p->toFileString() << "\n";
        }
        f.close();
    }

    static void chargerNotes(); // Implementation after class definitions
    static void sauvegarderNotes(); // Implementation after class definitions
    static void chargerPaiements(); // Implementation after class definitions

    static void sauvegarderPaiements() {
        ofstream f("paiements.txt");
        for (const auto& p : paiements)
            f << p.toFileString() << "\n";
        f.close();
    }

    static void chargerRelationsEtudiants(); // Implementation after class definitions
    static Etudiant* trouverEtudiant(int id); // Implementation after class definitions

    static Personne* trouverPersonne(int id) {
        for (auto* p : personnes)
            if (p->getId() == id) return p;
        return nullptr;
    }

    static bool estBloque(int id) {
        return tentativesEchouees.find(id) != tentativesEchouees.end() && 
               tentativesEchouees[id] >= 5;
    }
    
    static void enregistrerTentativeEchouee(int id) {
        if (tentativesEchouees.find(id) == tentativesEchouees.end()) {
            tentativesEchouees[id] = 1;
        } else {
            tentativesEchouees[id]++;
        }
    }
    
    static void reinitialiserTentatives(int id) {
        tentativesEchouees.erase(id);
    }
    
    static Personne* login(int id, string password){
        // Vérifier si l'ID est bloqué
        if (estBloque(id)) {
            throw runtime_error("Compte bloque apres 5 tentatives echouees");
        }
        
        for (auto* p : personnes) {
            if (p->getId() == id) {
                if (p->getPassword() == password) {
                    // Réinitialiser les tentatives en cas de succès
                    reinitialiserTentatives(id);
                    return p;
                } else {
                    // Enregistrer la tentative échouée
                    enregistrerTentativeEchouee(id);
                    throw runtime_error("Mot de Pass est Incorrect");
                }
            }
        }
        throw runtime_error("Person non trouve");
    }

    static Matiere* trouverMatiere(int id) {
        for (auto& m : matieres)
            if (m.getId() == id) return &m;
        return nullptr;
    }

    static Salle* trouverSalle(int id) {
        for (auto& s : salles)
            if (s.getId() == id) return &s;
        return nullptr;
    }

    static Enseignant* trouverEnseignant(int id); // Implementation after Enseignant class definition

    static Groupe* trouverGroupe(int id) {
        for (auto& g : groupes)
            if (g.getId() == id) return &g;
        return nullptr;
    }

    static void chargerGroupes();
    static void sauvegarderGroupes();
};

// Static member definitions
vector<Personne*> DataManager::personnes;
vector<Matiere> DataManager::matieres;
vector<Salle> DataManager::salles;
vector<Cours> DataManager::cours;
vector<Etudiant*> DataManager::etudiants;
vector<Note> DataManager::notes;
vector<Paiement> DataManager::paiements;
int DataManager::nextPersonneId = 1;
int DataManager::nextMatiereId = 1;
int DataManager::nextSalleId = 1;
int DataManager::nextCoursId = 1;
int DataManager::nextNoteId = 1;
int DataManager::nextPaiementId = 1;
int DataManager::nextGroupeId = 1;
vector<Groupe> DataManager::groupes;
map<int, int> DataManager::tentativesEchouees;

class Etudiant : public Personne {
private:
    vector<Matiere> matieres;
    vector<Note> notes;
    Emploi emploi;
    vector<Paiement> paiements;

public:
    Etudiant(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email), emploi(id) {}

    string getType() const override { return "Etudiant"; }

   void consulterMatieres() const {
    afficherTitreSection("MES MATIERES");
    if (DataManager::matieres.empty()) {
        cout << centrer("Aucune matiere.") << "\n";
    } else {
        for (const auto& m : DataManager::matieres)
            m.afficher();
    }
}

    void consulterNotes() const {
        afficherTitreSection("MES NOTES");
        if (notes.empty())
            cout << centrer("Aucune note.") << "\n";
        else {
            for (const auto& n : notes) {
                stringstream ss;
                ss << "Note [" << n.getId() << "] " << n.getValeur() 
                   << " | Date: " << n.getDate() << " | Matiere ID: " << n.getIdMatiere();
                cout << centrer(ss.str()) << "\n";
            }
        }
    }

    void consulterEmploi() const {
        emploi.consulter();
    }

    void consulterPaiements() const {
        afficherTitreSection("MES PAIEMENTS");
        if (paiements.empty())
            cout << centrer("Aucun paiement.") << "\n";
        else {
            for (const auto& p : paiements) {
                stringstream ss;
                ss << "Paiement [" << p.getId() << "] " << p.getMontant() 
                   << " DH | " << p.getDate() << " | Type: " << p.getType();
                cout << centrer(ss.str()) << "\n";
            }
        }
    }

    void ajouterPaiement(const Paiement& p) {
        paiements.push_back(p);
    }

    void ajouterCours(const Cours& c) {
        emploi.ajouterCours(c);
    }

    void ajouterNoteInterne(const Note& n) {
        notes.push_back(n);
    }

    void ajouterMatiere(const Matiere& m) {
        matieres.push_back(m);
    }

    friend class Enseignant;
    friend class Caissiere;
    friend class DirecteurPedagogique;
    friend class DataManager;

   void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU ETUDIANT ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Afficher mes informations") << "\n";
        cout << encadrerTexte("2. Consulter mes matieres") << "\n";
        cout << encadrerTexte("3. Consulter mes notes") << "\n";
        cout << encadrerTexte("4. Consulter mon emploi du temps") << "\n";
        cout << encadrerTexte("5. Consulter mes paiements") << "\n";
        cout << encadrerTexte("6. Consulter mes absences / retards") << "\n";
        string quitter = string(ROUGE) + "7. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 7)) continue;

        switch (choix) {
            case 1:
                afficherInfos();
                break;

            case 2:
                consulterMatieres();
                break;

            case 3:
                consulterNotes();
                break;

            case 4:
                consulterEmploi();
                break;

            case 5:
                consulterPaiements();
                break;

            case 6:
                cout << "\n";
                cout << centrer("Absences : " + to_string(getAbsence())) << "\n";
                cout << centrer("Retards  : " + to_string(getRetard())) << "\n";
                break;
        }

    } while (choix != 7);
}

};

class Bibliothecaire : public Personne {
public:
    Bibliothecaire(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Bibliothecaire"; }

    void enregistrerEmprunt(Etudiant& e, const string& livre) {
        ofstream f("bibliotheque.txt", ios::app);
        f << e.getId() << ";" << livre << ";" << "EMPRUNT\n";
        f.close();
        cout << centrer("Emprunt enregistre.") << "\n";
    }

    void enregistrerRetour(Etudiant& e, const string& livre) {
        ofstream f("bibliotheque.txt", ios::app);
        f << e.getId() << ";" << livre << ";" << "RETOUR\n";
        f.close();
        cout << centrer("Retour enregistre.") << "\n";
    }

    void consulterEmprunts() {
        ifstream f("bibliotheque.txt");
        if (!f.is_open()) {
            cout << centrer("Aucun emprunt enregistre.") << "\n";
            return;
        }
        afficherTitreSection("LISTE DES EMPRUNTS");
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string id, livre, type;
            getline(ss, id, ';');
            getline(ss, livre, ';');
            getline(ss, type, ';');
            cout << centrer("Etudiant " + id + " - " + livre + " (" + type + ")") << "\n";
        }
        f.close();
    }

   void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU BIBLIOTHECAIRE ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Enregistrer un emprunt") << "\n";
        cout << encadrerTexte("2. Enregistrer un retour") << "\n";
        cout << encadrerTexte("3. Consulter les emprunts") << "\n";
        string quitter = string(ROUGE) + "4. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 4)) continue;

        if (choix == 1 || choix == 2) {
            int idE;
            string livre;

            cout << centrer("ID Etudiant : ");
            idE = lireEntier();

            cout << centrer("Nom du livre : ");
            getline(cin, livre);

            Etudiant* e = DataManager::trouverEtudiant(idE);
            if (e) {
                if (choix == 1) {
                    enregistrerEmprunt(*e, livre);
                    cout << centrer("Emprunt enregistre avec succes.") << "\n";
                } else {
                    enregistrerRetour(*e, livre);
                    cout << centrer("Retour enregistre avec succes.") << "\n";
                }
            } else {
                cout << centrer("Etudiant non trouve.") << "\n";
            }
        }
        else if (choix == 3) {
            consulterEmprunts();
        }

    } while (choix != 4);
}


};

class Caissiere : public Personne {
public:
    Caissiere(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Caissiere"; }

    void encaisserPaiement(Etudiant& e, float montant, const string& type = "Frais De Scolarite") {
        string date;
        cout << centrer("Date (JJ/MM/AAAA) : ");
        cin >> date;
        Paiement p(DataManager::nextPaiementId++, e.getId(), montant, date, type);
        e.ajouterPaiement(p);
        p.sauvegarder();
        DataManager::paiements.push_back(p);
        cout << centrer("Paiement encaisse avec succes.") << "\n";
    }

    void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU CAISSIERE ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Encaisser un paiement") << "\n";
        cout << encadrerTexte("2. Consulter les paiements") << "\n";
        string quitter = string(ROUGE) + "3. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 3)) continue;

        if (choix == 1) {
            int idE;
            float montant;

            cout << centrer("ID Etudiant : ");
            idE = lireEntier();

            cout << centrer("Montant : ");
            montant = lireFloat();

            Etudiant* e = DataManager::trouverEtudiant(idE);
            if (e) {
                encaisserPaiement(*e, montant);
                cout << centrer("Paiement encaisse avec succes.") << "\n";
            } else {
                cout << centrer("Etudiant non trouve.") << "\n";
            }
        }
        else if (choix == 2) {
            afficherTitreSection("LISTE DES PAIEMENTS");
            for (const auto& p : DataManager::paiements) {
                stringstream ss;
                ss << "Paiement [" << p.getId() << "] " << p.getMontant() 
                   << " DH | " << p.getDate() << " | Type: " << p.getType();
                cout << centrer(ss.str()) << "\n";
            }
        }

    } while (choix != 3);
}

};

class PersonnelMenage : public Personne {
    string zone;

public:
    PersonnelMenage(int id, string prenom, string nom, string dateNaissance, string email, string z)
        : Personne(id, prenom, nom, dateNaissance, email), zone(z) {}

    string getType() const override { return "PersonnelMenage"; }
    string getZone() const { return zone; }

    string toFileString() const override {
        return zone + ";" + Personne::toFileString();
    }

    void nettoyer() {
        cout << centrer("Nettoyage de la zone : " + zone) << "\n";
        ofstream f("nettoyage.txt", ios::app);
        f << "Zone " << zone << " nettoyee le " << __DATE__ << "\n";
        f.close();
    }

   void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU PERSONNEL MENAGE ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Nettoyer ma zone") << "\n";
        cout << encadrerTexte("2. Afficher mes informations") << "\n";
        cout << encadrerTexte("3. Quitter") << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 3)) continue;

        if (choix == 1) {
            nettoyer();
            cout << centrer("Zone nettoyee avec succes.") << "\n";
        }
        else if (choix == 2) {
            afficherInfos();
        }

    } while (choix != 3);
}


};

class DirecteurPedagogique : public Personne {
public:
    DirecteurPedagogique(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "DirecteurPedagogique"; }

    void planifierCours(Enseignant& ens, const Cours& c) {
        c.sauvegarder();
        DataManager::cours.push_back(c);
        // Si le cours a un groupe, ajouter le cours aux étudiants du groupe
        if (c.getIdGroupe() > 0) {
            Groupe* g = DataManager::trouverGroupe(c.getIdGroupe());
            if (g) {
                for (int idE : g->getIdEtudiants()) {
                    Etudiant* e = DataManager::trouverEtudiant(idE);
                    if (e) {
                        e->ajouterCours(c);
                    }
                }
            }
        }
        cout << centrer("Cours planifie avec succes.") << "\n";
    }

   void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU DIRECTEUR PEDAGOGIQUE ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Planifier un cours") << "\n";
        cout << encadrerTexte("2. Consulter les cours") << "\n";
        cout << encadrerTexte("3. Consulter les matieres") << "\n";
        cout << encadrerTexte("4. Consulter les salles") << "\n";
        cout << encadrerTexte("5. Creer un groupe") << "\n";
        cout << encadrerTexte("6. Ajouter un etudiant a un groupe") << "\n";
        cout << encadrerTexte("7. Consulter les groupes") << "\n";
        string quitter = string(ROUGE) + "8. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 8)) continue;

        if (choix == 1) {
            int idEns, idM, idS, idG;
            string jour, hd, hf;

            cout << centrer("ID Enseignant : ");
            idEns = lireEntier();

            cout << centrer("ID Matiere : ");
            idM = lireEntier();

            cout << centrer("ID Salle : ");
            idS = lireEntier();

            cout << centrer("Jour : ");
            getline(cin, jour);

            cout << centrer("Heure debut : ");
            getline(cin, hd);

            cout << centrer("Heure fin : ");
            getline(cin, hf);

            cout << centrer("ID Groupe (0 si aucun groupe) : ");
            idG = lireEntier();

            Enseignant* ens = DataManager::trouverEnseignant(idEns);
            if (ens) {
                if (idG > 0) {
                    Groupe* g = DataManager::trouverGroupe(idG);
                    if (!g) {
                        cout << centrer("Groupe non trouve.") << "\n";
                        continue;
                    }
                }
                Cours c(DataManager::nextCoursId++, idM, idS, jour, hd, hf, idEns, idG);
                planifierCours(*ens, c);
            } else {
                cout << centrer("Enseignant non trouve.") << "\n";
            }
        }
        else if (choix == 2) {
            afficherTitreSection("LISTE DES COURS");
            for (const auto& c : DataManager::cours) {
                c.afficher();
            }
        }
        else if (choix == 3) {
            afficherTitreSection("LISTE DES MATIERES");
            for (const auto& m : DataManager::matieres) {
                stringstream ss;
                ss << "Matiere [" << m.getId() << "] " << m.getNom() 
                   << " (coef " << m.getCoefficient() << ") - " << m.getDescription();
                cout << centrer(ss.str()) << "\n";
            }
        }
        else if (choix == 4) {
            afficherTitreSection("LISTE DES SALLES");
            for (const auto& s : DataManager::salles) {
                stringstream ss;
                ss << "Salle [" << s.getId() << "] " << s.getNumero() 
                   << " | Cap: " << s.getCapacite() << " | Etage: " << s.getEtage()
                   << " | Etat: " << (s.estDisponible() ? "Libre" : "Occupee");
                cout << centrer(ss.str()) << "\n";
            }
        }
        else if (choix == 5) {
            string nom;
            cout << centrer("Nom du groupe : ");
            getline(cin, nom);
            
            if (!nom.empty()) {
                Groupe g(DataManager::nextGroupeId++, nom);
                DataManager::groupes.push_back(g);
                g.sauvegarder();
                DataManager::sauvegarderGroupes();
                cout << centrer("Groupe cree avec succes. ID: " + to_string(g.getId())) << "\n";
            } else {
                cout << centrer("Le nom du groupe ne peut pas etre vide.") << "\n";
            }
        }
        else if (choix == 6) {
            int idG, idE;
            cout << centrer("ID Groupe : ");
            idG = lireEntier();
            
            cout << centrer("ID Etudiant : ");
            idE = lireEntier();
            
            Groupe* g = DataManager::trouverGroupe(idG);
            if (g) {
                Etudiant* e = DataManager::trouverEtudiant(idE);
                if (e) {
                    // Vérifier si l'étudiant n'est pas déjà dans le groupe
                    bool dejaDansGroupe = false;
                    for (int id : g->getIdEtudiants()) {
                        if (id == idE) {
                            dejaDansGroupe = true;
                            break;
                        }
                    }
                    if (!dejaDansGroupe) {
                        g->ajouterEtudiant(idE);
                        DataManager::sauvegarderGroupes();
                        cout << centrer("Etudiant ajoute au groupe avec succes.") << "\n";
                    } else {
                        cout << centrer("Cet etudiant est deja dans le groupe.") << "\n";
                    }
                } else {
                    cout << centrer("Etudiant non trouve.") << "\n";
                }
            } else {
                cout << centrer("Groupe non trouve.") << "\n";
            }
        }
        else if (choix == 7) {
            afficherTitreSection("LISTE DES GROUPES");
            if (DataManager::groupes.empty()) {
                cout << centrer("Aucun groupe cree.") << "\n";
            } else {
                for (const auto& g : DataManager::groupes) {
                    g.afficher();
                    if (!g.getIdEtudiants().empty()) {
                        cout << centrer("Etudiants : ") << "\n";
                        for (int idE : g.getIdEtudiants()) {
                            Etudiant* e = DataManager::trouverEtudiant(idE);
                            if (e) {
                                cout << centrer("  - [" + to_string(idE) + "] " + e->getNomComplet()) << "\n";
                            }
                        }
                    }
                    cout << "\n";
                }
            }
        }

    } while (choix != 8);
}


};

class ResponsableSite : public Personne {
public:
    ResponsableSite(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "ResponsableSite"; }

    void signalerProbleme(const string& description) {
        ofstream f("maintenance.txt", ios::app);
        f << "PROBLEME;" << description << ";" << __DATE__ << "\n";
        f.close();
        cout << centrer("Probleme signale au service technique.") << "\n";
    }

    void consulterProblemes() {
        ifstream f("maintenance.txt");
        if (!f.is_open()) {
            cout << centrer("Aucun probleme signale.") << "\n";
            return;
        }
        afficherTitreSection("LISTE DES PROBLEMES");
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << centrer(line) << "\n";
        }
        f.close();
    }

    void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU RESPONSABLE SITE ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Signaler un probleme") << "\n";
        cout << encadrerTexte("2. Consulter les problemes") << "\n";
        string quitter = string(ROUGE) + "3. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 3)) continue;

        if (choix == 1) {
            string desc;
            cout << centrer("Description du probleme : ");
            getline(cin, desc);

            signalerProbleme(desc);
            cout << centrer("Probleme signale avec succes.") << "\n";
        }
        else if (choix == 2) {
            consulterProblemes();
        }

    } while (choix != 3);
}

};

class Technicien : public Personne {
public:
    Technicien(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Technicien"; }

    void reparer(const string& intervention) {
        ofstream f("maintenance.txt", ios::app);
        f << "REPARATION;" << intervention << ";" << __DATE__ << "\n";
        f.close();
        cout << centrer("Intervention technique enregistree.") << "\n";
    }

    void consulterInterventions() {
        ifstream f("maintenance.txt");
        if (!f.is_open()) {
            cout << centrer("Aucune intervention.") << "\n";
            return;
        }
        afficherTitreSection("LISTE DES INTERVENTIONS");
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << centrer(line) << "\n";
        }
        f.close();
    }

    void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU TECHNICIEN ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Enregistrer une reparation") << "\n";
        cout << encadrerTexte("2. Consulter les interventions") << "\n";
        string quitter = string(ROUGE) + "3. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 3)) continue;

        if (choix == 1) {
            string intervention;

            cout << centrer("Description de l'intervention : ");
            getline(cin, intervention);

            reparer(intervention);
            cout << centrer("Intervention enregistree avec succes.") << "\n";
        }
        else if (choix == 2) {
            consulterInterventions();
        }

    } while (choix != 3);
}

};

class SurveillantGeneral : public Personne {
public:
    SurveillantGeneral(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "SurveillantGeneral"; }

    void enregistrerAbsence(Personne& p) {
        p.ajouterAbsence();
        enregistrerAction("Absence", p.getId());
        DataManager::sauvegarderPersonnes();
        cout << centrer("Absence enregistree pour " + p.getNomComplet()) << "\n";
    }

    void enregistrerRetard(Personne& p) {
        p.ajouterRetard();
        enregistrerAction("Retard", p.getId());
        DataManager::sauvegarderPersonnes();
        cout << centrer("Retard enregistre pour " + p.getNomComplet()) << "\n";
    }

    void consulterabsence() {
        ifstream f("absence.txt");
        if (!f.is_open()) {
            cout << centrer("Aucun enregistrement.") << "\n";
            return;
        }
        afficherTitreSection("ENREGISTREMENTS ABSENCE");
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << centrer(line) << "\n";
        }
        f.close();
    }

    void affecterMatiere(Matiere* m, Etudiant& e){
        e.ajouterMatiere(*m);
    }
    
    

private:
    void enregistrerAction(const string& type, int idPersonne) {
        ofstream f("absence.txt", ios::app);
        f << type << ";" << idPersonne << ";" << __DATE__ << "\n";
        f.close();
    }

public:
   void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU SURVEILLANT GENERAL ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Enregistrer une absence") << "\n";
        cout << encadrerTexte("2. Enregistrer un retard") << "\n";
        cout << encadrerTexte("3. Consulter les enregistrements") << "\n";
        string quitter = string(ROUGE) + "4. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 4)) continue;

        if (choix == 1 || choix == 2) {
            int idP;
            cout << centrer("ID Personne : ");
            idP = lireEntier();

            Personne* p = DataManager::trouverPersonne(idP);
            if (p) {
                if (choix == 1) {
                    enregistrerAbsence(*p);
                    cout << centrer("Absence enregistree avec succes.") << "\n";
                } else {
                    enregistrerRetard(*p);
                    cout << centrer("Retard enregistre avec succes.") << "\n";
                }
            } else {
                cout << centrer("Personne non trouvee.") << "\n";
            }
        }
        else if (choix == 3) {
            consulterabsence();
        }

    } while (choix != 4);
}

};

class Enseignant : public Personne {
public:
    Enseignant(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Enseignant"; }

    void ajouterNote(Etudiant& e, const Note& n) {
        e.notes.push_back(n);
        n.sauvegarder(e.getId());
        DataManager::notes.push_back(n);
        cout << centrer("Note ajoutee avec succes.") << "\n";
    }

    void modifierNote(Etudiant& e, int index, float nouvelleValeur) {
        if (index >= 0 && index < (int)e.notes.size()) {
            e.notes[index].modifier(nouvelleValeur);
            cout << centrer("Note modifiee.") << "\n";
            DataManager::sauvegarderNotes();
        } else {
            cout << centrer("Index invalide.") << "\n";
        }
    }

    void supprimerNote(Etudiant& e, int index) {
        if (index >= 0 && index < (int)e.notes.size()) {
            e.notes.erase(e.notes.begin() + index);
            cout << centrer("Note supprimee.") << "\n";
            DataManager::sauvegarderNotes();
        } else {
            cout << centrer("Index invalide.") << "\n";
        }
    }

   void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU ENSEIGNANT ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Ajouter une note") << "\n";
        cout << encadrerTexte("2. Modifier une note") << "\n";
        cout << encadrerTexte("3. Supprimer une note") << "\n";
        cout << encadrerTexte("4. Consulter les etudiants") << "\n";
        cout << encadrerTexte("5. Consulter les matieres") << "\n";
        string quitter = string(ROUGE) + "6. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 6)) continue;

        if (choix == 1) {
            int idE, idM;
            float val;
            string date;

            cout << centrer("ID Etudiant : ");
            idE = lireEntier();

            cout << centrer("ID Matiere : ");
            idM = lireEntier();

            cout << centrer("Valeur de la note : ");
            val = lireFloat();

            cout << centrer("Date (JJ/MM/AAAA) : ");
            getline(cin, date);

            Etudiant* e = DataManager::trouverEtudiant(idE);
            if (e) {
                Note n(DataManager::nextNoteId++, val, date, idM);
                ajouterNote(*e, n);
                cout << centrer("Note ajoutee avec succes.") << "\n";
            } else {
                cout << centrer("Etudiant non trouve.") << "\n";
            }
        }
        else if (choix == 2) {
            int idE, index;
            float val;

            cout << centrer("ID Etudiant : ");
            idE = lireEntier();

            cout << centrer("Index de la note : ");
            index = lireEntier();

            cout << centrer("Nouvelle valeur : ");
            val = lireFloat();

            Etudiant* e = DataManager::trouverEtudiant(idE);
            if (e) {
                modifierNote(*e, index, val);
            } else {
                cout << centrer("Etudiant non trouve.") << "\n";
            }
        }
        else if (choix == 3) {
            int idE, index;

            cout << centrer("ID Etudiant : ");
            idE = lireEntier();

            cout << centrer("Index de la note : ");
            index = lireEntier();

            Etudiant* e = DataManager::trouverEtudiant(idE);
            if (e) {
                supprimerNote(*e, index);
            } else {
                cout << centrer("Etudiant non trouve.") << "\n";
            }
        }
        else if (choix == 4) {
            afficherTitreSection("LISTE DES ETUDIANTS");
            for (auto* e : DataManager::etudiants)
                e->afficherInfos();
        }
        else if (choix == 5) {
            afficherTitreSection("LISTE DES MATIERES");
            for (const auto& m : DataManager::matieres) {
                stringstream ss;
                ss << "Matiere [" << m.getId() << "] " << m.getNom() 
                   << " (coef " << m.getCoefficient() << ") - " << m.getDescription();
                cout << centrer(ss.str()) << "\n";
            }
        }

    } while (choix != 6);
}

};

// Implementation of DataManager::trouverEnseignant (after Enseignant class definition)
Enseignant* DataManager::trouverEnseignant(int id) {
    for (auto* p : personnes) {
        if (p->getId() == id && p->getType() == "Enseignant") {
            return dynamic_cast<Enseignant*>(p);
        }
    }
    return nullptr;
}

class Infirmier : public Personne {
public:
    Infirmier(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Infirmier"; }

    void enregistrerSoin(Personne& p, const string& soin) {
        ofstream f("infirmerie.txt", ios::app);
        f << p.getId() << ";" << p.getNomComplet() << ";" << soin << ";" << __DATE__ << "\n";
        f.close();
        cout << centrer("Soin enregistre pour " + p.getNomComplet()) << "\n";
    }

    void consulterSoins() {
        ifstream f("infirmerie.txt");
        if (!f.is_open()) {
            cout << centrer("Aucun soin enregistre.") << "\n";
            return;
        }
        afficherTitreSection("LISTE DES SOINS");
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << centrer(line) << "\n";
        }
        f.close();
    }

    void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU INFIRMIER ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Enregistrer un soin") << "\n";
        cout << encadrerTexte("2. Consulter les soins") << "\n";
        string quitter = string(ROUGE) + "3. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 3)) continue;

        if (choix == 1) {
            int idP;
            string soin;

            cout << centrer("ID Personne : ");
            idP = lireEntier();

            cout << centrer("Description du soin : ");
            getline(cin, soin);

            Personne* p = DataManager::trouverPersonne(idP);
            if (p) {
                enregistrerSoin(*p, soin);
                cout << centrer("Soin enregistre avec succes.") << "\n";
            } else {
                cout << centrer("Personne non trouvee.") << "\n";
            }
        }
        else if (choix == 2) {
            consulterSoins();
        }

    } while (choix != 3);
}

};

class AgentSecurite : public Personne {
public:
    AgentSecurite(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "AgentSecurite"; }

    void signalerIncident(const string& typeIncident, const string& lieu, const string& description) {
        ofstream f("securite.txt", ios::app);
        f << typeIncident << ";" << lieu << ";" << description << ";" << __DATE__ << "\n";
        f.close();
        cout << centrer("Incident de securite enregistre.") << "\n";
    }

    void consulterIncidents() {
        ifstream f("securite.txt");
        if (!f.is_open()) {
            cout << centrer("Aucun incident enregistre.") << "\n";
            return;
        }
        afficherTitreSection("LISTE DES INCIDENTS");
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << centrer(line) << "\n";
        }
        f.close();
    }

    void menu() override {
    int choix;
    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU AGENT DE SECURITE ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Signaler un incident") << "\n";
        cout << encadrerTexte("2. Consulter les incidents") << "\n";
        string quitter = string(ROUGE) + "3. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 3)) continue;

        if (choix == 1) {
            string type, lieu, desc;

            cout << centrer("Type d'incident : ");
            getline(cin, type);

            cout << centrer("Lieu : ");
            getline(cin, lieu);

            cout << centrer("Description : ");
            getline(cin, desc);

            signalerIncident(type, lieu, desc);
            cout << centrer("Incident enregistre avec succes.") << "\n";
        }
        else if (choix == 2) {
            consulterIncidents();
        }

    } while (choix != 3);
}

};

// Implementations of DataManager methods (after all class definitions)
void DataManager::chargerNotes() {
    ifstream f("notes.txt");
    if (!f.is_open()) return;
    notes.clear();
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        Note n;
        int idE;
        n.fromFileString(line, idE);
        notes.push_back(n);
        Etudiant* e = trouverEtudiant(idE);
        if (e) e->ajouterNoteInterne(n);
        if (n.getId() >= nextNoteId) nextNoteId = n.getId() + 1;
    }
    f.close();
}

void DataManager::sauvegarderNotes() {
    ofstream f("notes.txt");
    // Save notes from all students (DataManager is friend of Etudiant)
    for (auto* e : etudiants) {
        for (const auto& n : e->notes) {
            f << n.toFileString(e->getId()) << "\n";
        }
    }
    f.close();
}

void DataManager::chargerPaiements() {
    ifstream f("paiements.txt");
    if (!f.is_open()) return;
    paiements.clear();
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        Paiement p;
        p.fromFileString(line);
        paiements.push_back(p);
        Etudiant* e = trouverEtudiant(p.getIdEtudiant());
        if (e) e->ajouterPaiement(p);
        if (p.getId() >= nextPaiementId) nextPaiementId = p.getId() + 1;
    }
    f.close();
}

void DataManager::chargerGroupes() {
    ifstream f("groupes.txt");
    if (!f.is_open()) return;
    groupes.clear();
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        Groupe g;
        g.fromFileString(line);
        groupes.push_back(g);
        if (g.getId() >= nextGroupeId) nextGroupeId = g.getId() + 1;
    }
    f.close();
}

void DataManager::sauvegarderGroupes() {
    ofstream f("groupes.txt");
    for (const auto& g : groupes)
        f << g.toFileString() << "\n";
    f.close();
}

void DataManager::chargerRelationsEtudiants() {
    // Load student relations (matieres, notes, cours, paiements)
    for (auto* e : etudiants) {
        // Load matieres for student (simplified - would need separate file)
        // Load cours for student via groupes
        for (const auto& c : cours) {
            if (c.getIdGroupe() > 0) {
                Groupe* g = trouverGroupe(c.getIdGroupe());
                if (g) {
                    for (int idE : g->getIdEtudiants()) {
                        if (idE == e->getId()) {
                            e->ajouterCours(c);
                            break;
                        }
                    }
                }
            }
        }
    }
}

Etudiant* DataManager::trouverEtudiant(int id) {
    for (auto* e : etudiants)
        if (e->getId() == id) return e;
    return nullptr;
}

void DataManager::chargerPersonnes() {
    ifstream f("personnes.txt");
    if (!f.is_open()) return;
    personnes.clear();
    etudiants.clear();
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string type;
        getline(ss, type, ';');
        string data = line.substr(type.length() + 1);
        
        Personne* p = nullptr;
        if (type == "Etudiant") {
            Etudiant* e = new Etudiant(0, "", "", "", "");
            e->fromFileString(data);
            p = e;
            etudiants.push_back(e);
        } else if (type == "Enseignant") {
            p = new Enseignant(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "Caissiere") {
            p = new Caissiere(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "DirecteurPedagogique") {
            p = new DirecteurPedagogique(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "Bibliothecaire") {
            p = new Bibliothecaire(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "SurveillantGeneral") {
            p = new SurveillantGeneral(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "Infirmier") {
            p = new Infirmier(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "AgentSecurite") {
            p = new AgentSecurite(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "Technicien") {
            p = new Technicien(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "ResponsableSite") {
            p = new ResponsableSite(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "PersonnelMenage") {
            stringstream ss2(data);
            string zone;
            getline(ss2, zone, ';');
            string item;
            getline(ss2, item, ';'); int id = stoi(item);
            string prenom, nom, date, email, password, tel, adr;
            getline(ss2, prenom, ';');
            getline(ss2, nom, ';');
            getline(ss2, date, ';');
            getline(ss2, email, ';');
            getline(ss2, password, ';');
            getline(ss2, tel, ';');
            getline(ss2, adr, ';');
            getline(ss2, item, ';'); bool ac = (item == "1");
            getline(ss2, item, ';'); int a = stoi(item);
            getline(ss2, item, ';'); int r = stoi(item);
            PersonnelMenage* pm = new PersonnelMenage(id, prenom, nom, date, email, zone);
            pm->setPassword(password);
            pm->setTelephone(tel);
            pm->setAdresse(adr);
            pm->setActif(ac);
            pm->setAbsence(a);
            pm->setRetard(r);
            p = pm;
        }
        if (p) {
            personnes.push_back(p);
            if (p->getId() >= nextPersonneId) nextPersonneId = p->getId() + 1;
        }
    }
    f.close();
    chargerRelationsEtudiants();
}
int lireEntier() {
    int x;
    while (true) {
        cin >> x;
        if (!cin.fail()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << centrer("Entree invalide.") << "\n" << centrer("Entrez un nombre : ");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
bool verifierChoix(int choix, int min, int max) {
    if (choix < min || choix > max) {
        cout << centrer("Choix invalide. Reessayez.") << "\n";
        return false;
    }
    return true;
}
float lireFloat() {
    float x;
    while (true) {
        cin >> x;
        if (!cin.fail()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << centrer("Entree invalide. Entrez un nombre decimal : ");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
string centrer(const string& texte, int largeur ) {
    int espaces = (largeur - texte.length()) / 2;
    if (espaces > 0)
        return string(espaces, ' ') + texte;
    return texte;
}

void ligne() {
    cout << GRAS << BLEU << centrer("*****************************************") << RESET << "\n";
}

void dessinerLigneHaut() {
    cout << GRAS << BLEU;
    int espaces = (60 - 44) / 2;
    cout << string(espaces, ' ') << "+==========================================+" << string(espaces, ' ') << RESET << "\n";
}

void dessinerLigneBas() {
    cout << GRAS << BLEU;
    int espaces = (60 - 44) / 2;
    cout << string(espaces, ' ') << "+==========================================+" << string(espaces, ' ') << RESET << "\n";
}

void dessinerLigneSep() {
    cout << GRAS << BLEU;
    int espaces = (60 - 44) / 2;
    cout << string(espaces, ' ') << "+------------------------------------------+" << string(espaces, ' ') << RESET << "\n";
}

string encadrerTexte(const string& texte) {
    int largeurCadre = 42;
    int largeurTotale = 60;
    
    // Extraire le texte sans les codes de couleur pour calculer la longueur réelle
    string texteSansCouleur = texte;
    size_t pos;
    string codes[] = {"\033[0m", "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[36m", "\033[1m"};
    for (const auto& code : codes) {
        while ((pos = texteSansCouleur.find(code)) != string::npos) {
            texteSansCouleur.erase(pos, code.length());
        }
    }
    
    // Calculer les espaces pour centrer
    int longueurTexte = texteSansCouleur.length();
    int largeurDispo = largeurCadre - 4;
    int espacesGaucheTotal = (largeurTotale - largeurCadre) / 2;
    int espacesGaucheInterne = (largeurDispo - longueurTexte) / 2;
    int espacesDroiteInterne = largeurDispo - longueurTexte - espacesGaucheInterne;
    
    string result = string(espacesGaucheTotal, ' ') + GRAS + BLEU + "|" + RESET + "  " + 
                    string(espacesGaucheInterne, ' ') + texte + 
                    string(espacesDroiteInterne, ' ') + "  " + GRAS + BLEU + "|" + RESET;
    return result;
}

void afficherTitreSection(const string& titre) {
    cout << "\n";
    dessinerLigneHaut();
    string titreFormate = string(GRAS) + JAUNE + "**** " + titre + " ****" + RESET;
    cout << encadrerTexte(titreFormate) << "\n";
    dessinerLigneBas();
    cout << "\n";
}




int main() {
    cout << GRAS << VERT << centrer("**** SYSTEME DE GESTION SCOLAIRE ****") << RESET "\n\n";

    // Initialisation des fichiers
    vector<string> dbFiles = {
        "personnes.txt", "matieres.txt", "notes.txt", "salles.txt",
        "cours.txt", "paiements.txt", "bibliotheque.txt", "absence.txt",
        "maintenance.txt", "infirmerie.txt", "securite.txt", "nettoyage.txt", "groupes.txt"
    };

    for (const auto& filename : dbFiles) {
        ifstream check(filename);
        if (!check.is_open()) {
            ofstream create(filename);
            create.close();
        }
    }

    DataManager::chargerToutesLesDonnees();

    int choix;
    Personne* currentUser = nullptr;

    do {
        cout << "\n";
        dessinerLigneHaut();
        string titre = string(GRAS) + CYAN + "**** MENU PRINCIPAL ****" + RESET;
        cout << encadrerTexte(titre) << "\n";
        dessinerLigneSep();
        cout << encadrerTexte("1. Login") << "\n";
        string quitter = string(ROUGE) + "2. Quitter" + RESET;
        cout << encadrerTexte(quitter) << "\n";
        dessinerLigneBas();
        cout << centrer("Choix : ");

        choix = lireEntier();
        if (!verifierChoix(choix, 1, 2)) continue;

        if (choix == 1) {
            int id;
            cout << centrer("Entrez votre ID : ");
            id = lireEntier();

            // Vérifier si le compte est déjà bloqué
            if (DataManager::estBloque(id)) {
                cout << "\n";
                afficherTitreSection("COMPTE BLOQUE");
                cout << centrer("Ce compte est bloque apres 5 tentatives echouees.") << "\n";
                cout << centrer("Veuillez contacter l'administrateur.") << "\n";
                continue;
            }

            int tentatives = 0;
            bool connecte = false;
            int tentativesRestantes = 5;

            while (tentatives < 5 && !DataManager::estBloque(id)) {
                string password;
                cout << centrer("Entrez votre Mot de Passe : ");
                getline(cin, password);

                try {
                    currentUser = DataManager::login(id, password);
                    cout << "\n"<< GRAS << VERT << centrer("Bienvenue " + currentUser->getNomComplet() + " !") << RESET << "\n";
                    currentUser->menu();
                    connecte = true;
                    break;
                }
                catch (const runtime_error& e) {
                    string erreur = e.what();
                    if (erreur == "Compte bloque apres 5 tentatives echouees") {
                        cout << "\n";
                        afficherTitreSection("COMPTE BLOQUE");
                        cout << centrer("Trop de tentatives incorrectes.") << "\n";
                        cout << centrer("Ce compte est maintenant bloque.") << "\n";
                        cout << centrer("Veuillez contacter l'administrateur.") << "\n";
                        break;
                    } else {
                        tentatives++;
                        tentativesRestantes = 5 - tentatives;
                        cout << centrer("Mot de passe incorrect.") << "\n";
                        cout << centrer("Tentative " + to_string(tentatives) + " / 5") << "\n";
                        if (tentativesRestantes > 0) {
                            cout << centrer("Tentatives restantes : " + to_string(tentativesRestantes)) << "\n";
                        }
                        cout << "\n";
                    }
                }
            }

            if (!connecte && !DataManager::estBloque(id)) {
                cout << centrer("Trop de tentatives incorrectes.") << "\n";
                cout << centrer("Retour au menu principal...") << "\n";
            }
        }

    } while (choix != 2);

    DataManager::sauvegarderToutesLesDonnees();
    cout << "\n" << centrer("Donnees sauvegardees. Au revoir!") << "\n";

    for (auto* p : DataManager::personnes)
        delete p;

    return 0;
}



//compile in vs code by ameur
// g++ -std=c++11 final.cpp -o final