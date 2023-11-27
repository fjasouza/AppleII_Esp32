# AppleII no ESP32 (saida de video VGA e som)
![][1]

[1]: Imagens/AppleII_ESP32_Compara.jpg
***
**Apple II**
***

<p>É realmente incrivel o quanto o setor de embarcados evoluiu (e evolui), eu que entrei nessa area na época do surgimento do Apple II, que tive que aprender a programar o 6502 na unha e programava Z80 digitando o hex do código de maquina em um programador de eprom (somente quem é dessa época sabe do que to falando) acho impressionante como hoje podemos emular um 6502 e toda a engine de video de um Apple II dentro de um microcontrolador ESP32. Cheguei a duvidar quando, na época, me falaram que era possivel colocar todo o hardware de um Apple II dentro de um unico chip. (ainda não conhecia os FPGA´s na época)</p>
<p>Pois bem, eu, como amante nostalgico de Apple II e como programador, achei que seria interessante descobrir se já daria pra fazer isso em um ESP32. E acredito que tive sucesso nisso.</p>
<p>Ainda faltam alguns pormenores para resolver mas o principal foi feito, talvez com uma falha na paleta de cores e faltando alguns itens mas já é suficientemente ´jogável´.</p>
<p>Hoje como me falta tempo para dar continuidade e resolver esses pormenores resolvi disponibilizar o código fonte na sua totalidade para que outros continuem a labuta (ou usem o código como ferramenta de aprendizado).</p>

<p>Dentre esses pormenores, cito alguns:</p>

* Está emulando somente o Apple II (não emula seus 'descendentes').
* Só funciona com o disco 1 (não reconhece disco 2 mas é razoavelmente facil resolver isso).
* Só trabalha com a pagina 1 (Texto, baixa e alta resolução). Foi feito uma gambiarra para alguns jogos escreverem na pagina 1 como se fosse a pagina 2. (notei no jogo Rescue Raiders uma deficiencia nisso porque o sincronismo entre a varredura da memória e o video não estão corretos)
* Não emula um joystick (somente Teclado).
* Não acessa arquivos no SD Card (Esse Kit de ESP32 em especial aceita um SD Card, onde caberiam muitos discos de 1.44Mb).

<p>Provavelmente faltem alguns outros 'pormenores' que eu não me atentei mas que não acredito que faça falta para quem quer apenas matar saudade do Apple II </p>

<p>Deixei no pacote alguns jogos clássicos, já com a ROMs convertidas (arquivo xxx.DSK) para um array de bytes (estilo C).
(Nem sei se ainda existem direitos autorais sobre os jogos que deixei como exemplo mas como não estou vendendo nada e existem inumeros sites em que é possivel baixar essa ROMS, acredito que não tenha maior problema)</p>
<p>Para testa-las, basta retirar o comentário '//' ao lado dos #define do nome do jogo, no arquivo Esp32AppleII.ino (não esquecendo de deixar apenas um arquivo disponivel por compilação)  </p>

![][2]

[2]: Imagens/TelaInicial.png

***
<p>Alguns video funcionando:</p>

 * [DOS 3.3 (Youtube)](https://www.youtube.com/watch?v=gkJJiDuz0lA)
 * [Rescue Raiders (Youtube)](https://youtu.be/1CllMtIGst4)
 * [Galaxian (Youtube)](https://youtu.be/dFom_zQjH2I)
 * [ChopFliter (Youtube)](https://youtu.be/LiarlgUO_FE)
 * [CannonBall Blitz (Youtube)](https://youtu.be/a9vT981Lyd8)
 
<p>Para testar outras ROMS é preciso converter o arquivo hex deles em array de constantes na sintaxe da linguagem C, existem inumeras formas de se fazer essa conversão, basta procurar no google mas usei essa esse site em especial, que converte já deixando praticamente no jeito certo:</p>
<p>https://notisrac.github.io/FileToCArray<br></p>

<p>Digamos então, por exemplo, que queremos converter o jogo Galaxian, cujo arquivo de disco eu baixei com o nome Galaxian(1980)(Starcraft).dsk</p>

![][3]

[3]: Imagens/ConverteDsktoArrayC.png

<p>Depois bastaria clicar no botão 'save as file', nomear com extensão .h, adicionar ao projetos (como nos exemplos adicionados) e colocar o tamanho do array no código, como exemplificado abaixo:</p>

<p>de:   const unsigned char Galaxian_1980_Starcraft_[] PROGMEM</p>
<p>para: const unsigned char Galaxian_1980_Starcraft_[143360] PROGMEM</p>

<p>Deixei como desafio descobrirem porque o jogo LodeRunner não funcionou, não lembro se ele precisa acessar um disco 2 ou se acessa algum banco de memória que não implementei no projeto, mas fica ai o desafio.</p>

<p>Os arquivos CPU.INO e DISK.INO reaproveitei de projetos de outros autores e adaptei para o ESP32, assim que identifica-los colocarei aqui os devidos créditos.</p>

***
**ESP32**
***

<p>Vamos falar um pouco do poderoso ESP32, lançado em 2016 sendo um sucessor do famoso ESP8266, é um microcontrolador com dois 'cores' que pode rodar a incriveis 240Mhz (incriveis mesmo, comparado com o 1Mhz do saudoso 6502), usei um core para emular o 6502 e o outro core para emular a parte de video e tratamento de teclado.</p>

<p>Esse device também tem Wifi e bluetooth embutidos (já imaginaram um AppleII acessando a internet?)</p>

<p>Usei nesse projeto um kit facilmente encontrado no mercado (com muita variação no preço) que já vem com o ESP32 e conexão para entrada de teclado e mouse PS2, conector para saida de som, conector para SD Card, conector para saida VGA e conector USB (para programação e debug serial, entre outros) (vide imagem abaixo)</p>

![][4]

[4]: Imagens/Esp32_Front.jpeg

![][5]

[5]: Imagens/Esp32_Back.jpeg
 
<p>Para a parte gráfica, usei a ótima biblioteca open-source FabGL(http://www.fabglib.org) para ESP32.</p>

**Arduino IDE**

<p>Para quem é amante da linha AppleII mas não tem familiaridade com dispositivos embarcados (microcontroladores), em especial a linha Expressif (ESP32), deixo abaixo a configuração básica para programação utilizando a gratuita Arduino IDE (https://www.arduino.cc/en/software).</p> 

<p>Obs: Não vou me estender aqui como programar o ESP32 porque já existe muita informação na internet sobre como fazer isso.</p>

![][6]

[6]: Imagens/ConfigEsp32.png

![][7]

[7]: Imagens/ConfigEsp32_2.png

<p>Espero que esse Readme já seja suficiente para quem tenha um certo conhecimento ou desperte a curiosidade em muitos sobre programação do ESP32, sei que este é apenas 'mais um emulador' de Apple II e que, para windows, temos varios e de ótima qualidade e facilidade de uso, meu intuito foi mais estimular a prática e o conhecimento, espero que tenham gostado. boa sorte para todos.</p>

**Colaboração**

<p>Sou desenvolvedor autonomo então dependo dos meus trabalhos para tocar a vida (e as contas).</p>
<p>Se alguém achar que mereço uma cerveja (ou um cafezinho), meu pix e meu paypal é o email: fj_souza@hotmail.com</p>
<p>Se não achar ou não puder também não tem problema, usufrua e distribua conhecimentos.</p>

