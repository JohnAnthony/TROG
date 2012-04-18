COMPILATION INSTRUCTIONS
========================

PLEASE use an up-to-date compiler. This project uses a bunch of C++0x features
and older versions of GCC won't compile it. I suggest AT LEAST 4.7.0. Clang also
has been shown to compile from version 3.0 or later, although I'm making no
guarantees.

Dependencies
------------

Any version of ncurses from the last 10 years should do you. Do remember to get
the -devel package if you're using a Debian-based system (i.e. Ubuntu). I'm not
going to guarantee backwards compatibility for anything older than libncurses
5.9, though.

IF YOU HAVE PROBLEMS PLEASE MAKE SURE YOUR GCC AND LIBNCURSES ARE UP TO DATE

Your Terminal And You
---------------------

TROG assumes you have at least a 16-colour terminal. You may find it to your
liking to tweak colours to match how you like to play. If you are on an 8-colour
terminal you can pass TROG the flag -8 to make it play nice. The game also
expects at least an 80x25 terminal and can't operate on sizes smaller than that.
The program will gracefully quit in such a situation.

Resizing can cause graphical anomalies. If you really have to resize your
terminal while you're playing you can force a hard redraw by pressing F4 at any
of the main play mode screens. The game MAY automatically handle this itself,
but it relies upon your libncurses being compiled with the --enable-sigwinch
flag and I bet you don't know if it is or not.

---------------------------------------


KEYBINDINGS
===========

Mode Independent / Mode Change
------------------------------

    `   = Switch to Movement Mode
    l   = Look around
    @   = Character Screen
    i   = Inventory Screen
    p   = Drink a potion
    r   = Read book/scroll
    g   = Gear Screen (Switching up equipment)
    ?   = Game Info
    F1  = Help text (Not implemented)
    F4  = Force Redraw
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
    m = Cast magic (Not implemented)
    b = View spellbook (Not implemented)
    t = Talk/Interact with shopkeeper (Not implemented)

Map Look Mode
-------------

    qwe 789
    asd 456 = Move reticle
    zxc 123
    
    Arrow Keys = Move Camera

Inventory Screen
----------------

    v = View this item in more detail (Not implemented)
    d = Drop item (Not implemented)

Drink Menu
----------

    Return = Drink this potion / beverage
    v = View this item in more detail
    d = Drop item (Not implemented)

Read Menu
----------

    Return = Read this book / scroll
    v = View this item in more detail
    d = Drop item (Not implemented)

Gear Screen
-----------

    Return = Equip / unequip this item
    v = View this item in more detail
    d = Drop item (Not implemented)


---------------------------------------


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


---------------------------------------


RACES
=====

Dwarves
-------

Dwarves are hardy and resilient, but their stockyness makes them slow. They are
accustomed to the dark and their eyes see as far as any race's in the dungeon
halls. Dwarves are known for being particularly stubborn and their will and
innate magic resistance combine to make them particularly strong against magic.
They are never able to use magic themselves, however, regardless of class.

Dwarves love alcohol.

Elves
-----

Agile and alert, all elves possess an innate ability with magic. Their slight
frames makes them less hardy than the other races. Their keen senses give them
vision in the dark equalled only by the cave-dwelling dwarves.

Grey Orcs
---------

Traditionally barbarians from the wastelands, the noble Grey Orc is a creature
of cunning and strength -- their brutal nature makes them susceptible to magical
influence, however. Born as servants to a dark lord millenea ago, the race is
blessed with a good degree of low-light vision.

The clans often include shamen, sages and other magic users -- the orcs prize
strength above all, but strength has many more subtle forms than simply brawn.

Halflings
---------

Although physically unimposing, halflings are nimble and willful. Between their
short stature and love of good, home-cooked meals, they move slightly slower
than all but the stocky dwarves. They are favoured by luck, however, and on many
occasions a halfling has been known to miraculously avoid a danger that would
otherwise certainly have spelt their doom.

Half-Elf / Half Orc
-------------------

Children of mixed human and other racial heritage have half of their non-human
ancestor's traits and half of their human side's adaptability.

Humans
------

Humans, while less naturally talented in areas than the other races, also
suffer none particular disadvantages. They are incredibly adaptable and learn
to master their professions slightly faster than the other races.

Of all the races, however, humans are the least adapted to travelling without
a light source in the dark.

Lizardfolk
----------

Denizens of the swamps, lizardfolk are strong, resilient and skilled with
weapons. Their barbarous nature makes them susceptible to magic to a degree,
although not amazingly so. They can't breathe underwater, but they can dive for
minutes or, reputedly, even hours at a time without surfacing for air.

Though barbarous, their clans include shamen and priests.
