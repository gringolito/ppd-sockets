#!/bin/bash
# run_tests.sh
#
# "THE BEER-WARE LICENSE" (Revision 42):
# <filipeutzig@gmail.com> wrote this file. As long as you retain this
# notice you can do whatever you want with this stuff. If we meet some
# day, and you think this stuff is worth it, you can buy me a beer in
# return.
#
# Initial version by Filipe Utzig <filipeutzig@gmail.com> on 4/6/15.
#
# The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
# "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in
# this document are to be interpreted as described in RFC 2119.
#
# Script to automize parallel and sequential test execution
#

# Script vars
LOG_DIR=var/log
LOG_ERR=error.log
STATS_FILE=stats.log
STATS_BIN=var/confidence.py

# Programs vars
SEQ_BIN=bin/sequential
PAR_BIN=bin/parallel
MPI_BIN=mpirun
MPI_ARGS=-n

# Execution vars
VECTOR=var/big_vector.txt
EXECUTIONS=$(seq 1 29)
PROCESSORS="2 4 8 16"
ELEMENTS="100000 250000 500000"


print_usage() {
	echo -e "Usage: $0 OPTION"
	echo -e "\nOptions:"
	echo -e "\t--lad\tLAD environment setup"
	echo -e "\t--linux\tGeneric Linux environment setup (default)"
	echo -e "\t--help\tPrint this help message"
	exit 0
}

print_end() {
	echo -e "Errors can be found at $LOG_ERR"
	exit $1
}

if [ ! -x $SEQ_BIN ]; then
	echo -e "Invalid non-executable file $SEQ_BIN"
	exit 1
fi
if [ ! -x $PAR_BIN ]; then
	echo -e "Invalid non-executable file $PAR_BIN"
	exit 1
fi

if [ "$1" != "--lad" ] && [ "$1" != "--linux" ] && [ "$1" != "" ]; then
	print_usage
fi

if [ "$1" == "--lad" ]; then
	MPI_BIN=ladrun
	MPI_ARGS=-np
fi

if [ ! -d $LOG_DIR ]; then
	mkdir $LOG_DIR
fi

# Redirecting STDERR to a log file
exec 2>$LOG_ERR
############################################################################
# Sequential sampling
for i in $ELEMENTS; do
	LOG_FILE=$LOG_DIR/sequential_${i}.log
	echo -e "#Sequential - $i elements" > $LOG_FILE
	echo -ne "Running $SEQ_BIN with $i elements! Execution:  "
	for e in $EXECUTIONS; do
		if [ $e -gt 10 ]; then
			echo -ne "\b"
		fi
		echo -ne "\b$e"
		$SEQ_BIN $VECTOR $i >> $LOG_FILE
	done
	echo -e ""
	echo -e "\tExecution logs can be found at $LOG_FILE"
done

# Parallel sampling
for i in $ELEMENTS; do
	for p in $PROCESSORS; do
		LOG_FILE=$LOG_DIR/parallel_${p}_${i}.log
		echo -e "#Parallel - $p processors - $i elements" > $LOG_FILE
		echo -ne "Running $PAR_BIN - $p processors - $i elements! Execution:  "
		for e in $EXECUTIONS; do
			if [ $e -gt 10 ]; then
				echo -ne "\b"
			fi
			echo -ne "\b$e"
			$MPI_BIN $MPI_ARGS $p $PAR_BIN $VECTOR $i >> $LOG_FILE
		done
		echo -e ""
		echo -e "\tExecutions logs can be found at $LOG_FILE"
	done
done

echo -ne "Processing results"
$STATS_BIN $LOG_DIR $STATS_FILE
echo -e " [DONE]"
echo -e "\tStatistics can be found at $STATS_FILE"

print_end 0

