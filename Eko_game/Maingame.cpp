#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdio>
#include <cstdlib>

sf::Texture bgTexture;
sf::Texture player;
std::vector<sf::Texture> conteiner;
std::vector<sf::Texture> trash;
sf::Texture face1;
sf::Texture face2;

sf::Sprite bgSprite;
sf::Sprite playerSp;
std::vector<sf::Sprite> conteinerSp;
std::vector<sf::Sprite> trashSp;
sf::Sprite face1Sp;
sf::Sprite face2Sp;

sf::Font font;
sf::Text playerPoints;
sf::Text maxPoints;

void loadTextures() {
    if (!bgTexture.loadFromFile("images/wall/Game_1_wall.png")) printf("Blad tekstury tla");
    if (!player.loadFromFile("images/ludek.png"))               printf("Blad tekstury gracza");
    if (!face1.loadFromFile("images/faces.png", sf::IntRect(0, 0, 252, 206))) printf("Blad tekstury twarzy");
    if (!face2.loadFromFile("images/faces.png", sf::IntRect(252, 0, 252, 206))) printf("Blad tekstury twarzy");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            sf::Texture temp;
            if (!temp.loadFromFile("images/trash/all.png", sf::IntRect(300*j, 300*i, 300, 300))) printf("Blad tekstury smieci");
            trash.push_back(temp);
        }
    }
    for (int i = 0; i < 5; i++) {
        sf::Texture temp;
        if (!temp.loadFromFile("images/contener/all.png", sf::IntRect(500*i, 0, 500, 500))) printf("Blad tekstury smietnikow");
        conteiner.push_back(temp);
    }
}

sf::Vector2f positForConteiner(int i) {
    switch (i) {
    case 0: return sf::Vector2f(80.f, 0.f);
        break;
    case 1: return sf::Vector2f(1320.f, 0.f);
        break;
    case 2: return sf::Vector2f(1650.f, 400.f);
        break;
    case 3: return sf::Vector2f(50.f, 800.f);
        break;
    case 4: return sf::Vector2f(750.f, 740.f);
        break;
    }
}

void setSprites() {
    bgSprite.setTexture(bgTexture);
    playerSp.setTexture(player);
    playerSp.setPosition(1600.f, 700.f);
    face1Sp.setTexture(face1);
    face1Sp.setPosition(848.f, 0.f);
    face2Sp.setTexture(face2);
    face2Sp.setPosition(800.f, 0.f);
    for (int i = 0; i < 12; i++) {
        sf::Sprite temp;
        temp.setTexture(trash[i]);
        trashSp.push_back(temp);
        trashSp[i].setPosition(700.f, 100.f);
    }
    for (int i = 0; i < 5; i++) {
        sf::Sprite temp;
        temp.setTexture(conteiner[i]);
        conteinerSp.push_back(temp);
        conteinerSp[i].setScale(0.5f, 0.5f);
        conteinerSp[i].setPosition(positForConteiner(i));
    }
}

void loadText() {
    if (!font.loadFromFile("arial.ttf")) {
        printf("Blad fonta");
    }
    playerPoints.setFont(font);
    playerPoints.setString("0");
    playerPoints.setCharacterSize(50);
    playerPoints.setFillColor(sf::Color::Yellow);
    playerPoints.setPosition(600.f, 50.f);

    maxPoints.setFont(font);
    maxPoints.setString("/ 12");
    maxPoints.setCharacterSize(50);
    maxPoints.setFillColor(sf::Color::Yellow);
    maxPoints.setPosition(650.f, 50.f);
}

bool checkRightTrash(int conteinerIndex, int trashIndex) {
    switch (conteinerIndex) {
    case 0:
        if (trashIndex == 1 || trashIndex == 10) return true;
        else return false;
        break;
    case 1:
        if (trashIndex == 0 || trashIndex == 4 || trashIndex == 5 || trashIndex == 6 || trashIndex == 9) return true;
        else return false;
        break;
    case 2:
        if (trashIndex == 8) return true;
        else return false;
        break;
    case 3:
        if (trashIndex == 3 || trashIndex == 7) return true;
        else return false;
        break;
    case 4:
        if (trashIndex == 2 || trashIndex == 11) return true;
        else return false;
        break;
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Eco game");
    
    sf::Clock clock;
    float speed = 200.f;
    loadTextures();
    setSprites();
    loadText();
    int points = 0;

    bool smile = true;
    bool takeTrash = false;
    bool endGame = false;
    int trashInd = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (!endGame) {
            

            sf::Vector2f movePlayer;
            sf::Time elapsedTime = clock.getElapsedTime();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))         movePlayer += { -speed * elapsedTime.asSeconds(), 0.f };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))        movePlayer += { speed* elapsedTime.asSeconds(), 0.f };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))           movePlayer += { 0.f, -speed * elapsedTime.asSeconds() };
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))         movePlayer += { 0.f, speed* elapsedTime.asSeconds() };
            playerSp.move(movePlayer);
            clock.restart();

            if (playerSp.getGlobalBounds().intersects(trashSp[trashInd].getGlobalBounds()) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) takeTrash = true;
            if (takeTrash) trashSp[trashInd].move(movePlayer);
            for (int i = 0; i < 5; i++) {
                if (playerSp.getGlobalBounds().intersects(conteinerSp[i].getGlobalBounds()) && takeTrash && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    takeTrash = false;

                    if (checkRightTrash(i, trashInd)) {
                        points++;
                        playerPoints.setString(std::to_string(points));
                        smile = true;
                    }
                    else {
                        smile = false;
                    }

                    if (trashInd < 12) trashInd++;
                    if (trashInd >= 12) {
                        trashInd = 0;
                        endGame = true;
                    }
                }
            }

            window.clear();

            window.draw(bgSprite);
            for (auto it : conteinerSp) window.draw(it);
            window.draw(playerSp);
            if (!endGame) window.draw(trashSp[trashInd]);
            window.draw(playerPoints);
            window.draw(maxPoints);

            if (smile) window.draw(face2Sp);
            else window.draw(face1Sp);

            window.display();
        }
    }

    return 0;
}