/*
 * Built-in Function: long __builtin_expect(long exp,long c)
 * We may use __builtin_expect to provide the compiler with branch 
 * prediction information.In general,we should prefer to use actual profile
 * feedback for this(--fprofile-arcs),as programmers are notoriously bad 
 * at predicting how their program actually perform.However,there are 
 * applications in which this data is hard to collect.
 */
