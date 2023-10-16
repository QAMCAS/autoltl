#include <padkit.h>

static Chunk chunk[1] = { NOT_A_CHUNK };
static ChunkSet events[1] = { NOT_A_CHUNK_SET };

static void accumulateEventsFromFile(char const* filename) {
    DEBUG_ASSERT(isValid_chunk(chunk))
    DEBUG_ASSERT(isValid_cset(events))
    DEBUG_ERROR_IF(filename == NULL)

    FILE* const stream = fopen(filename, "r");
    if (stream == NULL) {
        fprintf(stderr, "\nCannot open %s\n\n", filename);
        exit(EXIT_FAILURE);
    }

    DEBUG_ERROR_IF(fromStream_chunk(chunk, stream, NULL) == 0xFFFFFFFF)
    NDEBUG_EXECUTE(fromStream_chunk(chunk, stream, NULL))

    DEBUG_ERROR_IF(fclose(stream) == EOF)
    NDEBUG_EXECUTE(fclose(stream))

    for (uint32_t i = 0; i < chunk->nStrings; i++) {
        char const* const event = get_chunk(chunk, i);
        DEBUG_ERROR_IF(event == NULL)

        uint64_t const event_len = strlen_chunk(chunk, i);
        if (event_len > 0) {
            DEBUG_ERROR_IF(addKey_cset(events, event, event_len) == 0xFFFFFFFF)
            NDEBUG_EXECUTE(addKey_cset(events, event, event_len))
        }
    }

    DEBUG_ASSERT_NDEBUG_EXECUTE(flush_chunk(chunk))
}

static void divideEventsFromFile(char const* filename) {
    DEBUG_ASSERT(isValid_chunk(chunk))
    DEBUG_ASSERT(isValid_cset(events))
    DEBUG_ERROR_IF(filename == NULL)

    FILE* const stream = fopen(filename, "r");
    if (stream == NULL) {
        fprintf(stderr, "\nCannot open %s\n\n", filename);
        exit(EXIT_FAILURE);
    }

    DEBUG_ERROR_IF(fromStream_chunk(chunk, stream, NULL) == 0xFFFFFFFF)
    NDEBUG_EXECUTE(fromStream_chunk(chunk, stream, NULL))

    for (uint32_t i = 0; i < chunk->nStrings; i++) {
        char const* const trace_element = get_chunk(chunk, i);
        DEBUG_ERROR_IF(trace_element == NULL)

        uint64_t const len = strlen_chunk(chunk, i);
        DEBUG_ERROR_IF(len == 0)

        puts(trace_element);
        if (getKeyId_cset(events, trace_element, len) != 0xFFFFFFFF)
            puts("..");
    }

    DEBUG_ERROR_IF(fclose(stream) == EOF)
    NDEBUG_EXECUTE(fclose(stream))

    DEBUG_ASSERT_NDEBUG_EXECUTE(flush_chunk(chunk))
}

static void free_stuff(void) {
    DEBUG_ABORT_IF(!free_chunk(chunk))
    NDEBUG_EXECUTE(free_chunk(chunk))
    DEBUG_ABORT_IF(!free_cset(events))
    NDEBUG_EXECUTE(free_cset(events))
}

static void showHelpMessage(void) {
    fputs("\n"
          "Usage: eventdivider <events_file> <trace_file>\n"
          "\n", stderr);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        showHelpMessage();
        return EXIT_SUCCESS;
    }

    DEBUG_ASSERT_NDEBUG_EXECUTE(
        constructEmpty_chunk(chunk, CHUNK_RECOMMENDED_PARAMETERS)
    )
    DEBUG_ASSERT_NDEBUG_EXECUTE(
        constructEmpty_cset(events, CHUNK_SET_RECOMMENDED_PARAMETERS)
    )
    DEBUG_ERROR_IF(atexit(free_stuff) != 0)
    NDEBUG_EXECUTE(atexit(free_stuff))

    accumulateEventsFromFile(argv[1]);

    divideEventsFromFile(argv[2]);

    return EXIT_SUCCESS;
}
