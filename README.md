COMPILATION INSTRUCTIONS
========================

PLEASE use an up-to-date compiler. This project uses a bunch of C++0x features
and older versions of GCC won't compile it. I suggest AT LEAST 4.7.0. clang also
has been shown to compile from version 3.0 or later, although I'm making no
guarantees.

Dependencies
------------

Any version of ncurses from the last 10 years should do you. Do remember to get
the -devel package if you're using a Debian-based system (i.e. Ubuntu). I'm not
going to guarantee backwards compatibility for anything older than libncurses
5.9, though.

IF YOU HAVE PROBLEMS PLEASE MAKE SURE YOUR GCC AND LIBNCURSES ARE UP TO DATE


            --> PAGE BREAK <--


KEYBINDINGS
===========

Mode Independent / Mode Change
------------------------------

`   = Switch to Movement Mode
l   = Look around
@   = Character Screen
i   = Inventory Screen
?   = Game Info
F1  = Help text (Not implemented)
F12 = Quit

Movement Mode
-------------

qwe 789
asd 456 = Movement
zxc 123

Arrow Keys = Move Camera
<  = Go up stairs
>  = Go down stairs

, = Pickup gold/items
p = Drink a potion
r = Read book/scroll
m = Cast magic (Not implemented)
b = View spellbook (Not implemented)
t = Talk/Interact with shopkeeper (Not implemented)

Map Look Mode
-------------

qwe 789
asd 456 = Move reticle
zxc 123

Arrow Keys = Move Camera


            --> PAGE BREAK <--


ITEMS
=====

Potions
-------

Potions come in varying potencies and varying effects. Potencies are, from least
to most,

    Minor,
    Lesser,
    Light,
    Moderate,
    Average,
    Strong

Effects are

    Healing         --  Recovers HP
    Enervation      --  Recovers MP
    Rejuvenation    --  Recovers both
