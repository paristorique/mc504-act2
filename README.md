mc504-act2
==========

Projeto 2 - The Dining Savages

Grupo:
Stéphane Ammar ra149071
Andréia Yukie Uratsuka ra096952

O problema escolhido foi o The Dining Savages. Pegamos um projeto do semestro pasado. Há um cozinheiro e uma certa quantidade de selvagens (de 2 até 10). Cada um deles é representado por uma thread diferente. Na tela inicial, é possível definir a quantidade de selvagens e a de refeições (entre 2 e 9) que o cozinheiro colocará no pot. Os selvagens estão jantando e somente um de cada vez pode ir até a panela pegar a comida. Se mais um selvagem ficar com fome, ele terá de esperar até que o outro termine de pegar a comida da panela. O acesso à panela é restrito através de locks, porém um selvagem pode ir pegar comida quando outro já estiver voltando, ou seja, quando o outro já estiver saído da panela. Quando a panela fica vazia, o cozinheiro é acordado para reabastecê-la. Os selvagens podem andar, comer ou ficar em “espera” (wander, um estado em que ele não está com fome). 
Para a animação, foram usadas matrizes, pois assim é possível modificar apenas as partes necessárias do desenho para dar a ideia de animação. 
Adicionamos as ameliorações seguintes:
- novo statu para os selvagens: "wander", seja fazer nada nem estar com fome
- pode ir pegar a comida enquanto alguem esta voltando
- os salvagens voltam mais rapido quando eles têm a comida
- os selvagens são educados : se um vizinho está indo pegando a comida enquanto o primeiro já está sentada com a comida, ele vai esperar que o vizinho volta para começar a comer
- tentativa de reduzir os problemas de starvation com a adição de um sleep.


