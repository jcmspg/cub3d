/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/08/27 18:05:48 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"


static bool create_array(t_map *map);
static bool convert_lines_to_array(t_map *map);

static char *normalize_line(const char *line, int width)
{
    // Remove newline first
    char *clean = ft_strdup(line);
    if (!clean)
        return NULL;
    
    int len = ft_strlen(clean);
    if (len > 0 && clean[len-1] == '\n')
        clean[len-1] = '\0';
    
    // Now normalize the cleaned line
    len = ft_strlen(clean);
    char *norm = malloc(width + 1);
    if (!norm)
    {
        free(clean);
        return NULL;
    }
    
    ft_memcpy(norm, clean, len);
    ft_memset(norm + len, ' ', width - len);  // Use space instead of VOID_SYMBOL
    norm[width] = '\0';
    
    free(clean);
    return norm;
}

static bool process_map_line(char **line, int *count, int *max_len)
{
    int len;

    printf("DEBUG: Processing map line: [%s]\n", *line);
    
    if (is_empty_line(*line))
    {
        printf("DEBUG: Map line is empty - this might be invalid\n");
        return false;  // Empty lines in map section are invalid
    }
    
    if (!is_valid_map_line(*line))
    {
        printf("DEBUG: Map line failed validation: [%s]\n", *line);
        return false;
    }
    
    printf("DEBUG: Map line is valid, counting it\n");
    (*count)++;
    len = ft_strlen(*line);
    if (len > *max_len)
        *max_len = len;
    return true;
}

static bool process_pre_map_line(char **line, bool *map_started, int *count)
{
    printf("DEBUG: Processing pre-map line: [%s]\n", *line);
    
    if (is_texture_or_color(*line))
    {
        printf("DEBUG: Line identified as texture/color\n");
        return true;
    }
    else if (is_empty_line(*line))
    {
        printf("DEBUG: Line identified as empty\n");
        return true;
    }
    else if (is_valid_map_line(*line))
    {
        printf("DEBUG: Line identified as valid map line - starting map\n");
        *map_started = true;
        if (count)
            (*count)++;
        return true;
    }
    else
    {
        printf("DEBUG: Line FAILED validation: [%s]\n", *line);
        ft_printf_fd(STDERR_FILENO, "invalid line [%s]\n", *line);
        return false;
    }
}

static bool process_line(char **line, bool *map_started, int *count, int *max_len)
{
    if (!*map_started)
        return process_pre_map_line(line, map_started, count);
    return process_map_line(line, count, max_len);
}

static bool populate_line(char **line, char **dest, t_map *map)
{
    if (is_empty_line(*line) || !is_valid_map_line(*line))
    {
        ft_printf_fd(STDERR_FILENO, "invalid map line [%s]\n", *line);
        // Don't free here - let main loop handle it
        return false;
    }
    *dest = normalize_line(*line, map->width);
    if (!*dest)
    {
        ft_printf_fd(STDERR_FILENO, "Memory error while duplicating line [%s]\n", *line);
        // Don't free here - let main loop handle it
        return false;
    }
    // Don't free here - let main loop handle it
    return true;
}

static bool populate_map_lines(char **line, bool *map_started, char **line2, t_map *map)
{
    if (!*map_started)
    {
        if (process_pre_map_line(line, map_started, NULL))
        {
            // If this line started the map, we need to process it as a map line too
            if (*map_started)
                return populate_line(line, line2, map);
            return true;
        }
        return false;
    }
    return populate_line(line, line2, map);
}

static bool count_lines_in_file(t_map *map)
{
    char    *line = NULL;
    bool    map_started = false;
    int     line_count = 0;
    int     max_len = 0;

    printf("DEBUG: Starting first pass through file\n");
    
    // First pass: count height and max width
    while ((line = get_next_line(map->fd)))
    {
        bool success = process_line(&line, &map_started, &line_count, &max_len);
        free(line);
        if (!success)
        {
            printf("DEBUG: First pass failed on line processing\n");
            return false;
        }
    }
    close(map->fd);

    printf("DEBUG: First pass complete. Lines counted: %d, Max width: %d\n", line_count, max_len);

    if (line_count == 0)
    {
        ft_putstr_fd("Error: No map lines found.\n", STDERR_FILENO);
        return false;
    }

    map->height = line_count;
    map->width = max_len;

    // Allocate map_lines array BEFORE second pass
    printf("DEBUG: Allocating map_lines array for %d lines\n", map->height);
    map->map_lines = ft_calloc(map->height + 1, sizeof(char *));
    if (!map->map_lines)
    {
        ft_putstr_fd("Error: Failed to allocate map_lines array.\n", STDERR_FILENO);
        return false;
    }

    // Reopen file for second pass
    printf("DEBUG: Reopening file for second pass\n");
    map->fd = open(map->filename, O_RDONLY);
    if (map->fd < 0)
    {
        ft_putstr_fd("Error: Failed to reopen map file.\n", STDERR_FILENO);
        return false;
    }

    printf("DEBUG: Starting second pass through file\n");
    // Second pass: populate map_lines
    int i = 0;
    map_started = false;
    while ((line = get_next_line(map->fd)))
    {
        char *clean = NULL;

        bool success = populate_map_lines(&line, &map_started, &clean, map);
        free(line);
        
        if (!success)
        {
            printf("DEBUG: Second pass failed on line processing\n");
            return false;
        }

        if (clean)
        {
            printf("DEBUG: Adding line %d to map_lines: [%s]\n", i, clean);
            
            if (i >= map->height)
            {
                ft_putstr_fd("Error: More lines than expected in map.\n", STDERR_FILENO);
                free(clean);
                return false;
            }
            
            map->map_lines[i++] = clean;
        }
    }
    
    close(map->fd);
    printf("DEBUG: Second pass complete. Populated %d lines\n", i);
    
    // FIRST: Create the array
    if (!create_array(map))
    {
        printf("DEBUG: Failed to create map array\n");
        return false;
    }
    
    // THEN: Convert map_lines to map_array
    if (!convert_lines_to_array(map))
    {
        printf("DEBUG: Failed to convert lines to array\n");
        return false;
    }
    
    return true;
}


static bool create_array(t_map *map)
{
    map->map_array = ft_calloc((map->height * map->width), sizeof(char));
    if (!map->map_array)
    {
        ft_putstr_fd("Error: Memory allocation failed for map array.\n", STDERR_FILENO);
        return false;
    }
    return true;
}

static bool convert_lines_to_array(t_map *map)
{
    if (!map || !map->map_lines || !map->map_array)
        return false;
    
    printf("DEBUG: Converting %d lines to map array\n", map->height);
    
    for (int y = 0; y < map->height; y++)
    {
        if (!map->map_lines[y])
        {
            printf("DEBUG: Warning - line %d is NULL\n", y);
            continue;
        }
        
        size_t line_len = ft_strlen(map->map_lines[y]);
        for (int x = 0; x < map->width; x++)
        {
            char c = ((size_t)x < line_len) ? map->map_lines[y][x] : ' ';
            map->map_array[y * map->width + x] = c;
        }
    }
    
    printf("DEBUG: Successfully converted map to array\n");
    return true;
}

bool create_map_array(t_cub_data *data)
{
    if (!data || !data->map || data->map->fd < 0)
    {
        ft_putstr_fd("Error: Invalid data structure.\n", STDERR_FILENO);
        return false;
    }
    if (!count_lines_in_file(data->map))
    {
        ft_putstr_fd("Error: Could not count map lines.\n", STDERR_FILENO);
        return false;
    }
    return true;
}

bool open_map(char *filename, t_cub_data *data)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        ft_putstr_fd("Error: Could not open map file.\n", STDERR_FILENO);
        return false;
    }

    data->map = ft_calloc(1, sizeof(t_map));
    if (!data->map)
    {
        close(fd);
        ft_putstr_fd("Error: Memory allocation failed for map.\n", STDERR_FILENO);
        return false;
    }

    data->map->fd = fd;
    data->map->filename = ft_strdup(filename);
    if (!data->map->filename)
    {
        close(fd);
        ft_putstr_fd("Error: Memory allocation failed.\n", STDERR_FILENO);
        return false;
    }

    if (!create_map_array(data))
    {
        close(fd);
        ft_putstr_fd("Error: Could not create map array.\n", STDERR_FILENO);
        return false;
    }

    return true;
}

int parse_cub_file(char *filename, t_cub_data *data)
{
    if (!filename || !data)
    {
        ft_printf_fd(STDERR_FILENO, "Error: Invalid arguments\n");
        return ERR_INVALID_ARG;
    }
    if (!open_map(filename, data))
    {
        ft_printf_fd(STDERR_FILENO, "Error opening map file\n");
        return ERR_FILE_NOT_FOUND;
    }
    return ERR_NO_ERROR;
}



// /**
//  * Parse texture paths from .cub file
//  */
// int	parse_textures(char *line, t_cub_data *data)
// {
// 	// TODO: Implement texture parsing
// 	(void)line;
// 	(void)data;
// 	return (0);
// }

// /**
//  * Parse color values (floor/ceiling) from .cub file
//  */
// int	parse_colors(char *line, t_cub_data *data)
// {
// 	// TODO: Implement color parsing
// 	(void)line;
// 	(void)data;
// 	return (0);
// }


bool look_for_spawn(t_map *map, int *spawn_x, int *spawn_y, char *direction)
{
    int i;


    if (!map || !map->map_array)
    {
        ft_putstr_fd("Error: Invalid map structure.\n", STDERR_FILENO);
        return (false);
    }

    i = 0;
    while(map->map_array[i])
    {
        if (map->map_array[i] == 'N' || map->map_array[i] == 'S' ||
            map->map_array[i] == 'E' || map->map_array[i] == 'W')
        {
            // direction
            *direction = map->map_array[i];
            // Convert 1D index to 2D grid coordinates
            *spawn_x = i % map->width;
            *spawn_y = i / map->width;
            printf("DEBUG: Player spawn found at grid (%d, %d)\n", *spawn_x, *spawn_y);
            return (true);
        }
        i++;
    }
    *spawn_x = -1;
    *spawn_y = -1;
    *direction = '\0';
    printf("DEBUG: No player spawn position found in map\n");
    ft_putstr_fd("Error: No player spawn position found in map.\n", STDERR_FILENO);
    
    
    
    return (false);
}
