S:=kodim23
CLI:=../src/cli
FILES:=$(S)_0.ppm $(S)_1.pgm $(S)_2.pgm $(S)_3.pgm $(S)_4.pbm $(S)_5.pbm $(S)_6.pbm $(S)_7.pbm $(S)_8.pbm $(S)_9.pbm  $(S)_10.pbm $(S)_11.pbm $(S)_12.pbm $(S)_13.pbm $(S)_14.pbm $(S)_15.pbm $(S)_16.pbm

.PHONY: all

all: $(FILES)

$(S)_0.ppm: $(S).png
	$(CLI) 0 $^ $@

$(S)_1.pgm: $(S)_0.ppm
	$(CLI) 1 $^ $@

$(S)_2.pgm: $(S)_0.ppm
	$(CLI) 2 $^ $@

$(S)_3.pgm: $(S)_0.ppm
	$(CLI) 3 $^ $@

# Note: in the following rules we could have used $(S)_1 or $(S)_3
# instead of $(S)_2.

$(S)_4.pbm: $(S)_2.pgm 
	$(CLI) 4 $^ $@

$(S)_5.pbm: $(S)_2.pgm
	$(CLI) 5 $^ $@

$(S)_6.pbm: $(S)_2.pgm
	$(CLI) 6 $^ $@

$(S)_7.pbm: $(S)_2.pgm
	$(CLI) 7 $^ $@

$(S)_8.pbm: $(S)_2.pgm
	$(CLI) 8 $^ $@

$(S)_9.pbm: $(S)_2.pgm
	$(CLI) 9 $^ $@

$(S)_10.pbm: $(S)_2.pgm
	$(CLI) 10 $^ $@

$(S)_11.pbm: $(S)_2.pgm
	$(CLI) 11 $^ $@

$(S)_12.pbm: $(S)_2.pgm
	$(CLI) 12 $^ $@

$(S)_13.pbm: $(S)_2.pgm
	$(CLI) 13 $^ $@

$(S)_14.pbm: $(S)_2.pgm
	$(CLI) 14 $^ $@

$(S)_15.pbm: $(S)_2.pgm
	$(CLI) 15 $^ $@

$(S)_16.pbm: $(S)_2.pgm
	$(CLI) 16 $^ $@


.PHONY: clean
clean:
	-rm -vf $(FILES)
