TARGET=_decimal
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
OBJECTS=$(TARGET)_conv.o $(TARGET)_other.o $(TARGET)_subs.o $(TARGET)_comp.o $(TARGET)_math.o

all: clean test

$(TARGET).a: $(OBJECTS)
	ar rcs $@ $^
	ranlib $@
	cp $@ lib$@

$(TARGET)_conv.o: $(TARGET)_converters.c $(TARGET).h
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET)_comp.o: $(TARGET)_compare.c $(TARGET).h
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET)_subs.o: $(TARGET)_subs.c $(TARGET).h
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET)_other.o: $(TARGET)_another.c $(TARGET).h
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET)_math.o: $(TARGET)_math.c $(TARGET).h
	$(CC) $(CFLAGS) -c -o $@ $<


test.o: tests.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: test.o $(TARGET).a
	$(CC) $(CFLAGS) $^ -lcheck -o $@
	./$@

gcov_report:
	$(CC) --coverage tests.c -o _test $(TARGET)_another.c $(TARGET)_compare.c $(TARGET)_converters.c $(TARGET)_subs.c $(TARGET)_math.c  -lcheck
	./_test
	lcov -t "_test" -o _test.info -c -d .
	genhtml -o report _test.info
	open report/index.html

cpplint:
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extensions=c *.c
	python3 ../materials/linters/cpplint.py --extensions=c *.h

clean:
	rm -f *.o test *.a
	rm -rf *.gcno *.gcda _test *.info
	rm -rf report
	rm -rf *.cfg
