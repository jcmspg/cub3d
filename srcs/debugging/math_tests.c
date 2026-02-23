/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 22:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:10:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void test_fixed_point(void) {
  t_fixed32 a;
  t_fixed32 b;
  t_fixed32 c;

  a = to_fixed32(10.5f);
  b = to_fixed32(2.0f);
  c = fixed32_mul(a, b);
  printf("10.5 * 2.0 = %f\n", from_fixed32(c));
  c = fixed32_div(a, b);
  printf("10.5 / 2.0 = %f\n", from_fixed32(c));
}