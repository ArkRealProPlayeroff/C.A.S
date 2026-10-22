#include <windows.h>
#include <shellapi.h>
#include <iostream>
#include <string>

int g_PageActive = 1;
int g_BoutonSurvole = 0;

int g_ChapitreSurvole = 0;
int g_LeconActive = 0;
int g_LeconPage = 1;

bool g_ChapitresTermines[4] = { false, false, false, false };
const char* g_TempsChapitres[4] = { "15 min", "25 min", "40 min", "10 min" };
const char* g_TitresChaps[4] = {
    "  1. Introduction et prerequis",
    "  2. Hello world of C++",
    "  3. Fonctions et librairies",
    "  4. Test final"
};

bool g_BoutonTerminerSurvole = false;
bool g_BoutonSuivantSurvole = false;
bool g_BoutonPrecedentSurvole = false;

const int LIEN_DASH_X_MIN = 340;
const int LIEN_DASH_Y_MIN = 420;
const int LIEN_DASH_X_MAX = 720;
const int LIEN_DASH_Y_MAX = 445;

const int LIEN_CHAP1_X_MIN = 330;
const int LIEN_CHAP1_Y_MIN = 260;
const int LIEN_CHAP1_X_MAX = 660;
const int LIEN_CHAP1_Y_MAX = 285;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case WM_SETCURSOR: {
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hwnd, &pt);

            if (g_PageActive == 1 && pt.x >= LIEN_DASH_X_MIN && pt.x <= LIEN_DASH_X_MAX && pt.y >= LIEN_DASH_Y_MIN && pt.y <= LIEN_DASH_Y_MAX) {
                SetCursor(LoadCursorA(NULL, IDC_HAND));
                return TRUE;
            }

            if (g_PageActive == 2 && g_LeconActive == 1 && g_LeconPage == 1 && pt.x >= LIEN_CHAP1_X_MIN && pt.x <= LIEN_CHAP1_X_MAX && pt.y >= LIEN_CHAP1_Y_MIN && pt.y <= LIEN_CHAP1_Y_MAX) {
                SetCursor(LoadCursorA(NULL, IDC_HAND));
                return TRUE;
            }

            return DefWindowProcA(hwnd, uMsg, wParam, lParam);
        }

        case WM_MOUSEMOVE: {
            int xSouris = LOWORD(lParam);
            int ySouris = HIWORD(lParam);
            int ancienBouton = g_BoutonSurvole;
            int ancienChapitre = g_ChapitreSurvole;
            bool ancienTerminer = g_BoutonTerminerSurvole;
            bool ancienSuivant = g_BoutonSuivantSurvole;
            bool ancienPrecedent = g_BoutonPrecedentSurvole;

            RECT rectClient;
            GetClientRect(hwnd, &rectClient);
            int largeurActuelle = rectClient.right - rectClient.left;
            int hauteurActuelle = rectClient.bottom - rectClient.top;

            int finCarteX = largeurActuelle - 50;
            int finCarteY = hauteurActuelle - 50;
            if (finCarteX < 350) finCarteX = 350;
            if (finCarteY < 300) finCarteY = 300;

            if (xSouris >= 0 && xSouris <= 260) {
                if (ySouris >= 110 && ySouris <= 150) g_BoutonSurvole = 1;
                else if (ySouris >= 160 && ySouris <= 200) g_BoutonSurvole = 2;
                else if (ySouris >= 210 && ySouris <= 250) g_BoutonSurvole = 3;
                else g_BoutonSurvole = 0;
                g_ChapitreSurvole = 0;
                g_BoutonTerminerSurvole = false;
                g_BoutonSuivantSurvole = false;
                g_BoutonPrecedentSurvole = false;
            }
            else if (g_PageActive == 2 && g_LeconActive == 0 && xSouris >= 320 && xSouris <= 750) {
                g_BoutonSurvole = 0;
                if (ySouris >= 170 && ySouris <= 210) g_ChapitreSurvole = 1;
                else if (ySouris >= 215 && ySouris <= 245) g_ChapitreSurvole = 2;
                else if (ySouris >= 260 && ySouris <= 300) g_ChapitreSurvole = 3;
                else if (ySouris >= 305 && ySouris <= 345) g_ChapitreSurvole = 4;
                else g_ChapitreSurvole = 0;
                g_BoutonTerminerSurvole = false;
                g_BoutonSuivantSurvole = false;
                g_BoutonPrecedentSurvole = false;
            }
            else if (g_PageActive == 2 && g_LeconActive != 0) {
                g_BoutonSurvole = 0;
                g_ChapitreSurvole = 0;

                int termX2 = finCarteX - 20;
                int termX1 = termX2 - 180;
                int termY2 = finCarteY - 20;
                int termY1 = termY2 - 40;

                g_BoutonTerminerSurvole = (xSouris >= termX1 && xSouris <= termX2 && ySouris >= termY1 && ySouris <= termY2);

                int prevX1 = termX1 - 250;
                int prevX2 = prevX1 + 110;
                int nextX1 = prevX2 + 10;
                int nextX2 = nextX1 + 110;

                int maxPages = (g_LeconActive == 1) ? 1 : 3;

                g_BoutonPrecedentSurvole = (g_LeconPage > 1 && xSouris >= prevX1 && xSouris <= prevX2 && ySouris >= termY1 && ySouris <= termY2);
                g_BoutonSuivantSurvole = (g_LeconPage < maxPages && xSouris >= nextX1 && xSouris <= nextX2 && ySouris >= termY1 && ySouris <= termY2);
            }
            else {
                g_BoutonSurvole = 0;
                g_ChapitreSurvole = 0;
                g_BoutonTerminerSurvole = false;
                g_BoutonSuivantSurvole = false;
                g_BoutonPrecedentSurvole = false;
            }

            if (g_BoutonSurvole != ancienBouton || g_ChapitreSurvole != ancienChapitre ||
                g_BoutonTerminerSurvole != ancienTerminer || g_BoutonSuivantSurvole != ancienSuivant ||
                g_BoutonPrecedentSurvole != ancienPrecedent) {
                InvalidateRect(hwnd, NULL, FALSE);
            }
            return 0;
        }

        case WM_LBUTTONDOWN: {
            int xSouris = LOWORD(lParam);
            int ySouris = HIWORD(lParam);
            bool actionEffectuee = false;

            if (g_BoutonSurvole != 0) {
                g_PageActive = g_BoutonSurvole;
                g_LeconActive = 0;
                g_LeconPage = 1;
                actionEffectuee = true;
            }
            else if (g_PageActive == 2 && g_ChapitreSurvole != 0) {
                g_LeconActive = g_ChapitreSurvole;
                g_LeconPage = 1;
                actionEffectuee = true;
            }
            else if (g_PageActive == 2 && g_LeconActive != 0 && g_BoutonTerminerSurvole) {
                g_ChapitresTermines[g_LeconActive - 1] = true;
                g_LeconActive = 0;
                g_LeconPage = 1;
                g_BoutonTerminerSurvole = false;
                actionEffectuee = true;
            }
            else if (g_PageActive == 2 && g_LeconActive != 0 && g_BoutonSuivantSurvole) {
                int maxPages = (g_LeconActive == 1) ? 1 : 3;
                if (g_LeconPage < maxPages) {
                    g_LeconPage++;
                }
                actionEffectuee = true;
            }
            else if (g_PageActive == 2 && g_LeconActive != 0 && g_BoutonPrecedentSurvole) {
                if (g_LeconPage > 1) {
                    g_LeconPage--;
                }
                actionEffectuee = true;
            }
            else if (g_PageActive == 1 && xSouris >= LIEN_DASH_X_MIN && xSouris <= LIEN_DASH_X_MAX && ySouris >= LIEN_DASH_Y_MIN && ySouris <= LIEN_DASH_Y_MAX) {
                ShellExecuteA(NULL, "open", "https://learncpp.com", NULL, NULL, SW_SHOWNORMAL);
                actionEffectuee = true;
            }
            else if (g_PageActive == 2 && g_LeconActive == 1 && g_LeconPage == 1 && xSouris >= LIEN_CHAP1_X_MIN && xSouris <= LIEN_CHAP1_X_MAX && ySouris >= LIEN_CHAP1_Y_MIN && ySouris <= LIEN_CHAP1_Y_MAX) {
                ShellExecuteA(NULL, "open", "https://www.jetbrains.com/fr-fr/clion/download/", NULL, NULL, SW_SHOWNORMAL);
                actionEffectuee = true;
            }

            if (actionEffectuee) {
                InvalidateRect(hwnd, NULL, FALSE);
            }
            return 0;
        }

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                DestroyWindow(hwnd);
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rectClient;
            GetClientRect(hwnd, &rectClient);
            int largeurActuelle = rectClient.right - rectClient.left;
            int hauteurActuelle = rectClient.bottom - rectClient.top;

            HBRUSH hFondBrush = CreateSolidBrush(RGB(240, 244, 248));
            FillRect(hdc, &rectClient, hFondBrush);
            DeleteObject(hFondBrush);

            RECT rectSidebar = {0, 0, 260, hauteurActuelle};
            HBRUSH hSidebarBrush = CreateSolidBrush(RGB(24, 34, 45));
            FillRect(hdc, &rectSidebar, hSidebarBrush);
            DeleteObject(hSidebarBrush);

            HFONT hFontTitre = CreateFontA(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
            HFONT hFontTexte = CreateFontA(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
            HFONT hFontBold = CreateFontA(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
            SetBkMode(hdc, TRANSPARENT);

            SelectObject(hdc, hFontTitre);
            SetTextColor(hdc, RGB(255, 255, 255));
            TextOutA(hdc, 25, 30, "KOGITOS:C++ COURS", 17);

            SelectObject(hdc, hFontTexte);
            int yPositions[3] = {110, 160, 210};
            const char* nomsBoutons[3] = { "  Dashboard", "  Chapitres", "  A propos" };

            for (int i = 0; i < 3; i++) {
                int idBouton = i + 1;
                RECT rectBouton = {10, yPositions[i], 250, yPositions[i] + 40};

                if (g_BoutonSurvole == idBouton) {
                    HBRUSH hHoverBrush = CreateSolidBrush(RGB(45, 60, 78));
                    FillRect(hdc, &rectBouton, hHoverBrush);
                    DeleteObject(hHoverBrush);
                    SetTextColor(hdc, RGB(255, 255, 255));
                }
                else if (g_PageActive == idBouton) {
                    HBRUSH hActiveBrush = CreateSolidBrush(RGB(0, 102, 204));
                    FillRect(hdc, &rectBouton, hActiveBrush);
                    DeleteObject(hActiveBrush);
                    SetTextColor(hdc, RGB(255, 255, 255));
                }
                else {
                    SetTextColor(hdc, RGB(160, 170, 185));
                }
                TextOutA(hdc, 25, yPositions[i] + 10, nomsBoutons[i], static_cast<int>(strlen(nomsBoutons[i])));
            }

            SelectObject(hdc, hFontTitre);
            SetTextColor(hdc, RGB(24, 34, 45));
            if (g_PageActive == 1) TextOutA(hdc, 300, 30, "Tableau de Bord", 15);
            else if (g_PageActive == 2) TextOutA(hdc, 300, 30, "Chapitres du Cours", 19);
            else if (g_PageActive == 3) TextOutA(hdc, 300, 30, "A propos de Kogitos", 19);

            int finCarteX = largeurActuelle - 50;
            int finCarteY = hauteurActuelle - 50;
            if (finCarteX < 350) finCarteX = 350;
            if (finCarteY < 300) finCarteY = 300;

            RECT carteContenu = {300, 140, finCarteX, finCarteY};
            HBRUSH hCarteBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &carteContenu, hCarteBrush);
            FrameRect(hdc, &carteContenu, (HBRUSH)GetStockObject(GRAY_BRUSH));
            DeleteObject(hCarteBrush);

            SelectObject(hdc, hFontTexte);

            if (g_PageActive == 1) {
                SetTextColor(hdc, RGB(40, 40, 40));
                TextOutA(hdc, 330, 170, "Voici la progression de votre apprentissage:", 44);

                SelectObject(hdc, hFontBold);
                SetTextColor(hdc, RGB(0, 102, 204));
                TextOutA(hdc, 340, 220, "CHAPITRE", 8);
                TextOutA(hdc, 780, 220, "STATUT", 6);
                HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                HPEN hAncienPen = (HPEN)SelectObject(hdc, hPen);
                MoveToEx(hdc, 330, 245, NULL);
                LineTo(hdc, finCarteX - 30, 245);

                SelectObject(hdc, hFontTexte);
                int yTableau = 260;
                for (int i = 0; i < 4; i++) {
                    SetTextColor(hdc, RGB(40, 40, 40));
                    TextOutA(hdc, 340, yTableau, g_TitresChaps[i] + 2, static_cast<int>(strlen(g_TitresChaps[i]) - 2));

                    if (g_ChapitresTermines[i]) {
                        SetTextColor(hdc, RGB(46, 125, 50));
                        TextOutA(hdc, 780, yTableau, "Termine [X]", 11);
                    } else {
                        SetTextColor(hdc, RGB(239, 108, 0));
                        TextOutA(hdc, 780, yTableau, "En cours [ ]", 12);
                    }
                    yTableau += 35;
                }
                SelectObject(hdc, hAncienPen);
                DeleteObject(hPen);

                HFONT hFontLien = CreateFontA(18, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
                HFONT hOldFont = (HFONT)SelectObject(hdc, hFontLien);
                SetTextColor(hdc, RGB(0, 102, 204));
                TextOutA(hdc, LIEN_DASH_X_MIN, LIEN_DASH_Y_MIN, "Documentation complete : Ouvrir LearnCPP.com ->", 47);
                SelectObject(hdc, hOldFont);
                DeleteObject(hFontLien);
                SetTextColor(hdc, RGB(0, 0, 0));
                TextOutA(hdc, 330, 460, "Lien d'un cours plus detailler mais moins significatif que la production de F.Abdelwakil!", 89);
                TextOutA(hdc, 330, 480, "Tous droits reservees", 21);
            }
            else if (g_PageActive == 2) {
                if (g_LeconActive == 0) {
                    int yChaps[4] = {170, 215, 260, 305};

                    for (int i = 0; i < 4; i++) {
                        int idChap = i + 1;
                        RECT rectChap = {320, yChaps[i], finCarteX - 20, yChaps[i] + 40};

                        if (g_ChapitreSurvole == idChap) {
                            HBRUSH hHoverChap = CreateSolidBrush(RGB(230, 242, 255));
                            FillRect(hdc, &rectChap, hHoverChap);
                            DeleteObject(hHoverChap);
                            SetTextColor(hdc, RGB(0, 102, 204));
                        } else {
                            SetTextColor(hdc, RGB(40, 40, 40));
                        }

                        std::string titreComplet = g_TitresChaps[i];
                        if (g_ChapitresTermines[i]) titreComplet += "  (Valide)";

                        TextOutA(hdc, 330, yChaps[i] + 10, titreComplet.c_str(), static_cast<int>(titreComplet.length()));
                    }
                }
                else {
                    SetTextColor(hdc, RGB(0, 102, 204));
                    SelectObject(hdc, hFontBold);

                    if (g_LeconActive == 1) {
                        TextOutA(hdc, 330, 180, "CHAPITRE 1 : INTRODUCTION ET PREREQUIS (Page 1/1)", 49);
                        SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(60, 60, 60));

                        TextOutA(hdc, 330, 225, "Veuillez d'abord installer CLION avec le lien ci-dessous :", 58);

                        HFONT hFontLienChap = CreateFontA(18, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
                        HFONT hOldFont = (HFONT)SelectObject(hdc, hFontLienChap);
                        SetTextColor(hdc, RGB(0, 102, 204));
                        TextOutA(hdc, LIEN_CHAP1_X_MIN, LIEN_CHAP1_Y_MIN, "Telecharger JetBrains CLion (Site officiel) ->", 45);
                        SelectObject(hdc, hOldFont);
                        DeleteObject(hFontLienChap);

                        SetTextColor(hdc, RGB(60, 60, 60));
                        TextOutA(hdc, 330, 295, "CLION est une plateforme facile a utiliser pour coder en language C/C#/C++ etc..", 80);
                        TextOutA(hdc, 330, 315, "D'abords il faut creer un nouveau projet et choisisez C++/20 pour commencer", 75);
                        TextOutA(hdc, 330, 335, "Ensuite tu peut voir un fichier crée apelée main.cpp, soit sur que tu soit dans l'editeur du fichier", 100);
                        TextOutA(hdc, 330, 355, "Bravo vous avec terminé ce chapitre maintenant vous pouvez cliquer sur le boutton terùinée en bas", 97);
                    }
                    else if (g_LeconActive == 2) {
                        if (g_LeconPage == 1) {
                            TextOutA(hdc, 330, 180, "CHAPITRE 2 : HELLO WORLD OF C++ (Page 1/3)", 42);
                            SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(60, 60, 60));
                            TextOutA(hdc, 330, 230, "D'abords il faut ecrire #include <iostream> dans la premiere ligne.", 67);
                            TextOutA(hdc, 330, 250, "Cette commande sera executer par le preprocesseur elle dit au programme avant d'executer d'inclure cette librairie.", 115);
                            TextOutA(hdc, 330, 270, "iostream est une librairie qui te permet d'utiliser des commandes utiles pour programmer en c++ cele-ci fais partie des librairies standards", 140);
                            TextOutA(hdc, 330, 290, "cette librairie permet de manipuler les donner entrantes et sortantes de la console.", 84);
                            TextOutA(hdc, 330, 310, "par exemple avec cout << tu peut faire sortit un texte que tu doit mettre entre guillemets \"ici\"", 98);
                            TextOutA(hdc, 330, 330, "Mais d'abords il faut cree une fonction principale de type int on parlera plus tard des types de donnees.", 105);
                            TextOutA(hdc, 330, 350, "Ecrivez : int main() {", 22);
                            TextOutA(hdc, 330, 370, "Puis ecrivez : std::cout << \"Hello World\": pour emettre du texte.", 67);
                            TextOutA(hdc, 330, 390, "Puis ecrivez :return 0: car les fonctions de types int doivent retourner une valeur a la fin plus de detaille sur les fonctions dans le chapitre suivant.", 153);
                            TextOutA(hdc, 330, 410, "Et enfin : } pour arreter la fonction", 37);
                            TextOutA(hdc, 330, 430, "Maintenant vous devez cliquer sur le button run pour executer le programme.", 75);
                        } else if (g_LeconPage == 2) {
                            TextOutA(hdc, 330, 180, "CHAPITRE 2 : HELLO WORLD OF C++ (Page 2/3)", 42);
                            SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(60, 60, 60));
                            TextOutA(hdc, 330, 230, "Pour programmer en c++ il faut connaitre les types de donnees il sont fondamentaux (par exemple le texte que vous voyez est une donnee):", 136);
                            TextOutA(hdc, 330, 250, "int: c'est les nombres entiers comme 1, 2;", 42);
                            TextOutA(hdc, 330, 270, "float: c'est des nombres decimales simples comme 0.01;", 54);
                            TextOutA(hdc, 330, 290, "double: aussi des nombres decimaux plus precis que float;", 57);
                            TextOutA(hdc, 330, 310, "bool: elle store soit true(vrai ou 1) et false(faux ou 0);", 58);
                            TextOutA(hdc, 330, 330, "char: permet de contenir un character 'ici'", 45);
                            TextOutA(hdc, 330, 350, "string: permet de contenir plusieurs characteres \"ici\".", 57);
                            TextOutA(hdc, 330, 370, "mais d'abords il faut initialiser une variable de cette maniere:", 64);
                            TextOutA(hdc, 330, 390, "typededonnee nomdelavariable = donee;", 37);
                            TextOutA(hdc, 330, 410, "Ex: int variable = 1;", 21);
                            TextOutA(hdc, 330, 430, "ici ci on traduit en francais cela donne: creer une place (dans la memoire) pour un nombre entier nomme variable est asigne a la valeur 1", 137);
                            TextOutA(hdc, 330, 450, "Tu peut aussi juste declarer puis asigner une valeur apres", 76);
                            TextOutA(hdc, 330, 470, "Ex:Int variable;", 16);
                            TextOutA(hdc, 330, 490, "   Variable = 1;", 16);
                        } else {
                            TextOutA(hdc, 330, 180, "CHAPITRE 2 : HELLO WORLD OF C++ (Page 3/3)", 42);
                            SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(60, 60, 60));
                            TextOutA(hdc, 330, 230, "Pour lire des donant rentrantes a la console on peut utiliser cin>>;", 68);
                            TextOutA(hdc, 330, 250, "Pour exercice vous devez faire un programme qui demande a l'utilisateur son age", 79);
                            TextOutA(hdc, 330, 270, "Utiliser cout<< pour demander a l'utilisateur d'ecrire son age", 62);
                            TextOutA(hdc, 330, 290, "Puis creer une donne age pour placer l'age ecrit par l'utilisateur en utilisant cin>>", 85);
                            TextOutA(hdc, 330, 310, "------------------------------[Correction]------------------------------", 72);
                            TextOutA(hdc, 330, 330, "------------------------------------------------------------------------", 72);
                            TextOutA(hdc, 330, 350, "#include <iostream>", 19);
                            TextOutA(hdc, 330, 370, "", 0);
                            TextOutA(hdc, 330, 390, "int main() {", 12);
                            TextOutA(hdc, 330, 410, "   cout << \"Ecrivez votre age: \" << endl;", 43);
                            TextOutA(hdc, 330, 430, "   int userage;", 15);
                            TextOutA(hdc, 330, 450, "   cin >> userage;", 18);
                            TextOutA(hdc, 330, 470, "   return 0;", 12);
                            TextOutA(hdc, 330, 490, "}", 1);
                        }
                    }
                    else if (g_LeconActive == 3) {
                        if (g_LeconPage == 1) {
                            TextOutA(hdc, 330, 180, "CHAPITRE 3 : FONCTIONS ET LIBRAIRIES (Page 1/3)", 47);
                            SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(60, 60, 60));
                            TextOutA(hdc, 330, 230, "Les fonctions decoupent logiquement votre script.", 49);
                            TextOutA(hdc, 330, 250, "Il permet de reutiliser des lignes de codes pour rendre le code plus lisible et simple.", 87);
                            TextOutA(hdc, 330, 270, "Pour declarer une fonction il faut lui attribuer un type de donne qu'il retournera", 82);
                            TextOutA(hdc, 330, 290, "Par exemple si il est de type int a la fin il doit retourner une valeur de type int.", 84);
                            TextOutA(hdc, 330, 310, "Mais en utilisant le type void la fonction ne retournera pas de valeur cela peut etre utile dans certeins cas.", 110);
                            TextOutA(hdc, 330, 330, "Voici la maniere dont on declare une fonction:", 46);
                            TextOutA(hdc, 330, 350, "Typededeonne Nomdelafonction(lesdonee, apasser);", 48);
                            TextOutA(hdc, 330, 370, "certaines fonction ont besoins de donnes qu'il faut passer dans les paranthese.", 79);
                            TextOutA(hdc, 330, 390, "Par exemple une fonction qui multiplie deux nombres:", 52);
                            TextOutA(hdc, 330, 410, "Int multiply(int a, int b) {", 28);
                            TextOutA(hdc, 330, 430, "   int z = a * b;", 17);
                            TextOutA(hdc, 330, 450, "   return z;", 11);
                            TextOutA(hdc, 330, 470, "}", 1);
                            TextOutA(hdc, 330, 490, "cette fonction retourne z qui est a * b.", 40);
                        } else if (g_LeconPage == 2) {
                            TextOutA(hdc, 330, 180, "CHAPITRE 3 : FONCTIONS ET LIBRAIRIES (Page 2/3)", 47);
                            SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(60, 60, 60));
                            TextOutA(hdc, 330, 230, "Maintenant qu'on sait faire une fonction, voyant comment l'utiliser dans le main.", 81);
                            TextOutA(hdc, 330, 250, "Pour appeler une fonction il suffit d'ecrire son nom et de lui donner les valeurs entre paranthese.", 96);
                            TextOutA(hdc, 330, 270, "Exemple: int resultat = multiplie(5, 3);", 41);
                            TextOutA(hdc, 330, 290, "Le programme va executer le code de la fonction et stocker le resultat dans la variable.", 88);
                            TextOutA(hdc, 330, 310, "L'ordre du passage des variables est important en C++", 53);
                            TextOutA(hdc, 330, 330, "Si tu inverse les deux nombres le resultat peut changer selon ce que fait ta fonction.", 86);
                            TextOutA(hdc, 330, 350, "Tu peux aussi inclure d'autres librairies comme <cmath> pour faire des calculs mathematiques.", 94);
                            TextOutA(hdc, 330, 370, "Par exemple avec sqrt() pour calculer une racine carree.", 55);
                            TextOutA(hdc, 330, 390, "Ainsi la librairie dans son code definit et initialise la fonction sqrt", 71);
                            TextOutA(hdc, 330, 410, "Pour que dans ton code tu na pas besoins de reecrire cette fonction de zero", 75);

                        } else {
                            TextOutA(hdc, 330, 180, "CHAPITRE 3 : FONCTIONS ET LIBRAIRIES (Page 3/3)", 47);
                            SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(60, 60, 60));
                            TextOutA(hdc, 330, 230, "Pour exercice tu doit creer une fonction qui calcule le carre d'un nombre entier.", 83);
                            TextOutA(hdc, 330, 250, "Utilise le type int pour la fonction et pour le nombre rentre en parametre.", 78);
                            TextOutA(hdc, 330, 270, "------------------------------[Correction]------------------------------", 72);
                            TextOutA(hdc, 330, 290, "------------------------------------------------------------------------", 72);
                            TextOutA(hdc, 330, 310, "#include <iostream>", 19);
                            TextOutA(hdc, 330, 330, "", 0);
                            TextOutA(hdc, 330, 350, "int carre(int nombre) {", 22);
                            TextOutA(hdc, 330, 370, "   return nombre * nombre;", 26);
                            TextOutA(hdc, 330, 390, "}", 1);
                            TextOutA(hdc, 330, 410, "int main() {", 12);
                            TextOutA(hdc, 330, 430, "   int resultat = carre(4);", 27);
                            TextOutA(hdc, 330, 450, "   return 0;", 12);
                            TextOutA(hdc, 330, 470, "}", 1);
                        }
                    }
                    else if (g_LeconActive == 4) {
                            TextOutA(hdc, 330, 180, "CHAPITRE 4 : TEST FINAL (Page 1/3)", 34);
                            SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(60, 60, 60));
                            TextOutA(hdc, 330, 230, "Maintenant que tu as les bases il faudrais tester tes competances", 65);
                            TextOutA(hdc, 330, 250, "Il faudra que tu creer un code qui simule une banque en ligne", 61);
                            TextOutA(hdc, 330, 270, "Creer une interface basique dans la console pas un truc trop avance", 67);
                            TextOutA(hdc, 330, 290, "Quelque chose de simple qui permet de deposer et retirer de l'argent", 68);
                            TextOutA(hdc, 330, 310, "IL ne doit pas etre possible de retirer de l'argent avec un solde negatif", 73);
                            TextOutA(hdc, 330, 330, "Bonne Chance!", 13);
                    }

                    SelectObject(hdc, hFontTexte);

                    int termX2 = finCarteX - 20;
                    int termX1 = termX2 - 180;
                    int termY2 = finCarteY - 20;
                    int termY1 = termY2 - 40;

                    RECT rectTerminer = {termX1, termY1, termX2, termY2};
                    HBRUSH hBtnBrush = CreateSolidBrush(g_BoutonTerminerSurvole ? RGB(38, 166, 91) : RGB(46, 125, 50));
                    FillRect(hdc, &rectTerminer, hBtnBrush);
                    DeleteObject(hBtnBrush);

                    SetTextColor(hdc, RGB(255, 255, 255));
                    TextOutA(hdc, termX1 + 20, termY1 + 10, "Terminer la lecon", 17);

                    int prevX1 = termX1 - 250;
                    int prevX2 = prevX1 + 110;
                    int nextX1 = prevX2 + 10;
                    int nextX2 = nextX1 + 110;

                    int maxPages = (g_LeconActive == 1 || g_LeconActive == 4) ? 1 : 3;

                    if (g_LeconPage > 1) {
                        RECT rectPrev = {prevX1, termY1, prevX2, termY2};
                        HBRUSH hPrevBrush = CreateSolidBrush(g_BoutonPrecedentSurvole ? RGB(70, 80, 95) : RGB(45, 60, 78));
                        FillRect(hdc, &rectPrev, hPrevBrush);
                        DeleteObject(hPrevBrush);
                        SetTextColor(hdc, RGB(255, 255, 255));
                        TextOutA(hdc, prevX1 + 15, termY1 + 10, "< Precedent", 11);
                    }

                    if (g_LeconPage < maxPages) {
                        RECT rectNext = {nextX1, termY1, nextX2, termY2};
                        HBRUSH hNextBrush = CreateSolidBrush(g_BoutonSuivantSurvole ? RGB(0, 122, 235) : RGB(0, 102, 204));
                        FillRect(hdc, &rectNext, hNextBrush);
                        DeleteObject(hNextBrush);
                        SetTextColor(hdc, RGB(255, 255, 255));
                        TextOutA(hdc, nextX1 + 20, termY1 + 10, "Suivant >", 9);
                    }
                }
            }
            else if (g_PageActive == 3) {
                int ySection = 170;

                SelectObject(hdc, hFontBold); SetTextColor(hdc, RGB(0, 102, 204));
                TextOutA(hdc, 330, ySection, "1. DOCUMENTATION", 16);
                SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(40, 40, 40));
                TextOutA(hdc, 330, ySection + 25, "Cette application utilise l'API native Win32 (Windows.h) pour le rendu d'UI et les connaissance de F.Abdelwakil en C++.", 119);

                ySection += 75;
                SelectObject(hdc, hFontBold); SetTextColor(hdc, RGB(0, 102, 204));
                TextOutA(hdc, 330, ySection, "2. BIBLIOGRAPHIE", 16);
                SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(40, 40, 40));
                TextOutA(hdc, 330, ySection + 25, "- Guide complet de programmation C++ : LearnCPP.com", 51);
                TextOutA(hdc, 330, ySection + 50, "- Microsoft Developer Network (MSDN) : Win32 API Windowing", 58);

                ySection += 100;
                SelectObject(hdc, hFontBold); SetTextColor(hdc, RGB(0, 102, 204));
                TextOutA(hdc, 330, ySection, "3. REMERCIEMENTS", 16);
                SelectObject(hdc, hFontTexte); SetTextColor(hdc, RGB(40, 40, 40));
                TextOutA(hdc, 330, ySection + 25, "Merci d'utiliser Kogitos une plateforme faite par F.Abdelwakil", 62);
            }

            DeleteObject(hFontTitre);
            DeleteObject(hFontTexte);
            DeleteObject(hFontBold);

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char CLASS_NAME[] = "Kogitos";

    WNDCLASSA wc = {};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursorA(NULL, IDC_ARROW);

    RegisterClassA(&wc);

    RECT wr = {0, 0, 1280, 720};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Kogitos",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 1;

    ShowWindow(hwnd, SW_SHOW);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}