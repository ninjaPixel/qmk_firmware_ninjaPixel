# Genesis

This keymap directory was created by running:
```shell
qmk new-keymap -kb sofle/rev1 -km ninjaPixel_sofle_rev1

# output:
Ψ Generating a new keymap


Configure Development Board
For more information, see:
https://docs.qmk.fm/feature_converters

Use converter?
	1. No (default)
	2. bit_c_pro
	3. blok
	4. bonsai_c3
	5. bonsai_c4
	6. elite_pi
	7. helios
	8. imera
	9. kb2040
	10. liatris
	11. michi
	12. promicro_rp2040
	13. proton_c
	14. rp2040_ce
	15. sparkfun_pm2040
	16. stemcell
	17. svlinky
Please enter your choice: [1]

# Choose option 1
```

# Steps to compile and flash


```shell
qmk compile -kb sofle/rev1 -km ninjaPixel_sofle_rev1
```

# flash half a board
```shell
qmk flash -kb sofle/rev1 -km ninjaPixel_sofle_rev1
```
When the above runs, it will then be searching for the keyboard.
Single tap the reset button (just above the TRRS jack), and it will be found and flashed.
Disconnect the USB port from the keyboard and then plug it into the other half (TRRS cable can remain attached throughout), and then run the flash command again.
Once both halfs are flashed, make sure you plug the USB cable back into the left hand keyboard.
