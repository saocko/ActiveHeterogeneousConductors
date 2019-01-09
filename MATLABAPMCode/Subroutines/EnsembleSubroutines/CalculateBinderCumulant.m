function[binder_cum] = CalculateBinderCumulant(vector, error_bars)


vector = MakeHorizontal(vector)';
error_bars = MakeHorizontal(error_bars)';

norm_vector = MakeHorizontal(vector)/sum(vector);
first_ord_array = 1:length(vector);
centered_first_order = first_ord_array - sum(norm_vector .* first_ord_array);
sec_order_array  = centered_first_order .^2;
fourth_order_array = centered_first_order.^4;

sec_moment = sum(norm_vector.*sec_order_array);
fourth_moment = sum(norm_vector .*fourth_order_array);

binder_cum = 1 - fourth_moment/(3 * sec_moment * sec_moment);

