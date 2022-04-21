CC=gcc
CFLAGS='-Wall -Werror -Wshadow -std=gnu99 -g -I/opt/local/include -I/usr/include/gsl'
LDFLAGS='-lm -pg -g -lgsl -L/opt/local/lib -lfftw3 -largtable2 -std=gnu99'
OMP_NUM_THREADS=2

#analyze: analyze.c
#${CC} analyze.c -o analyze ${CFLAGS} ${LDFLAGS}

#generate_source: generate_source.c
${CC} generate_source.c -o generate_source ${CFLAGS} ${LDFLAGS}

#prober: prober.c
${CC} prober.c -o prober ${CFLAGS} ${LDFLAGS}
