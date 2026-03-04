IPA constant propagation start:

IPA structures before propagation:

Jump functions:
  Jump functions of caller  printf/126:
  Jump functions of caller  main/125:
    callsite  main/125 -> printf/126 : 
       no arg info
  Jump functions of caller  process_batch/124:
  Jump functions of caller  apply_tax/123:

 Propagating constants:

Not considering process_batch/124 for cloning; no hot calls.
Not considering apply_tax/123 for cloning; no hot calls.

overall_size: 25

IPA lattices after all propagation:

Lattices:
  Node: main/125:
  Node: process_batch/124:
    param [0]: BOTTOM
         ctxs: BOTTOM
         Bits unusable (BOTTOM)
         [unsupported_range] VARYING
        AGGS BOTTOM
    param [1]: BOTTOM
         ctxs: BOTTOM
         Bits unusable (BOTTOM)
         [unsupported_range] VARYING
        AGGS BOTTOM
  Node: apply_tax/123:
    param [0]: BOTTOM
         ctxs: BOTTOM
         Bits unusable (BOTTOM)
         [unsupported_range] VARYING
        AGGS BOTTOM

IPA decision stage:


IPA constant propagation end

Reclaiming functions:
Reclaiming variables:
Clearing address taken flags:
Symbol table:

printf/126 (printf)
  Type: function
  Visibility: semantic_interposition external public
  References: 
  Referring: 
  Availability: not_available
  Function flags:
  Called by: main/125 (10737416 (estimated locally),1.00 per call) 
  Calls: 
main/125 (main)
  Type: function definition analyzed
  Visibility: externally_visible semantic_interposition public
  References: GLOBAL_FEE/122 (read) 
  Referring: 
  Availability: available
  Function flags: count:10737416 (estimated locally) body only_called_at_startup executed_once
  Called by: 
  Calls: printf/126 (10737416 (estimated locally),1.00 per call) 
process_batch/124 (process_batch)
  Type: function definition analyzed
  Visibility: externally_visible semantic_interposition public
  References: 
  Referring: 
  Availability: available
  Function flags: count:1073741824 (estimated locally) body
  Called by: 
  Calls: 
apply_tax/123 (apply_tax)
  Type: function definition analyzed
  Visibility: externally_visible semantic_interposition public
  References: 
  Referring: 
  Availability: available
  Function flags: count:1073741824 (estimated locally) body
  Called by: 
  Calls: 
GLOBAL_FEE/122 (GLOBAL_FEE)
  Type: variable definition analyzed
  Visibility: externally_visible semantic_interposition public
  References: 
  Referring: main/125 (read) 
  Availability: available
  Varpool flags: initialized

;; Function apply_tax (apply_tax, funcdef_no=122, decl_uid=4311, cgraph_uid=123, symbol_order=123)

Modification phase of node apply_tax/123
int apply_tax (int amount)
{
  int _2;

  <bb 2> [local count: 1073741824]:
  _2 = amount_1(D) + 5;
  return _2;

}
;; Function process_batch (process_batch, funcdef_no=123, decl_uid=4315, cgraph_uid=124, symbol_order=124)

Modification phase of node process_batch/124
int process_batch (int count, int rate)
{
  int _1;
  int _4;

  <bb 2> [local count: 1073741824]:
  _1 = count_2(D) * rate_3(D);
  _4 = _1 * 2;
  return _4;

}
;; Function main (main, funcdef_no=124, decl_uid=4317, cgraph_uid=125, symbol_order=125) (executed once)

Modification phase of node main/125
int main ()
{
  int j;
  int i;
  long long int total_balance;
  int GLOBAL_FEE.0_1;
  long long int _2;

  <bb 2> [local count: 10737416]:
  goto <bb 4>; [100.00%]

  <bb 3> [local count: 1063004408]:
  total_balance_11 = total_balance_3 + 158;
  GLOBAL_FEE.0_1 = GLOBAL_FEE;
  _2 = (long long int) GLOBAL_FEE.0_1;
  total_balance_12 = _2 + total_balance_11;
  i_13 = i_5 + 1;

  <bb 4> [local count: 1073741824]:
  # total_balance_3 = PHI <0(2), total_balance_12(3)>
  # i_5 = PHI <0(2), i_13(3)>
  if (i_5 != 1000000)
    goto <bb 3>; [99.00%]
  else
    goto <bb 8>; [1.00%]

  <bb 8> [local count: 10737416]:
  goto <bb 6>; [100.00%]

  <bb 5> [local count: 1063004408]:
  total_balance_9 = total_balance_4 + 100;
  j_10 = j_6 + 1;

  <bb 6> [local count: 1073741824]:
  # total_balance_4 = PHI <total_balance_3(8), total_balance_9(5)>
  # j_6 = PHI <0(8), j_10(5)>
  if (j_6 != 1000000)
    goto <bb 5>; [99.00%]
  else
    goto <bb 7>; [1.00%]

  <bb 7> [local count: 10737416]:
  printf ("Final Balance: %lld\n", total_balance_4);
  return 0;

}


