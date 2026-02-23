/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_values.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/01 17:55:31 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEY_VALUES_H
# define KEY_VALUES_H

# if defined(__APPLE__) && defined(__MACH__)
#  define KEY_W 13
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_ESC 53
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_UP 126
#  define KEY_DOWN 125
#  define KEY_SPACE 49
#  define KEY_SHIFT 257
# else
#  define KEY_W 119
#  define KEY_A 97
#  define KEY_S 115
#  define KEY_D 100
#  define KEY_ESC 65307
#  define KEY_LEFT 65361
#  define KEY_RIGHT 65363
#  define KEY_UP 65362
#  define KEY_DOWN 65364
#  define KEY_SPACE 32
#  define KEY_SHIFT 65505
# endif

#endif
