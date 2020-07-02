#include <stdio.h>
#include <libubus.h>
#include <libubox/list.h>
#include <libubox/blobmsg_json.h>

#include "rt.h"

static struct ubus_context *ctx;
static struct blob_buf b;
static int ctx_ref;

enum
{
    CMD_ID,
    CMD_MSG,
    CMD_MAX,
};

static const struct blobmsg_policy cmd_policy[CMD_MAX] = {
    [CMD_ID]	= { .name = "id", 	.type = BLOBMSG_TYPE_INT32 },
    [CMD_MSG]	= { .name = "msg", 	.type = BLOBMSG_TYPE_STRING }
};

// ubus call basicstation echo '{"id":1,"msg":"some_message"}'
static int station_echo(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req,
                      const char *method, struct blob_attr *msg)
{
    struct blob_attr *tb[CMD_MAX];
    int tmp_id;
    char *tmp_msg = NULL;

    blobmsg_parse(cmd_policy, CMD_MAX, tb, blob_data(msg), blob_len(msg));

    blob_buf_init(&b, 0);
    if (tb[CMD_ID])
    {
        tmp_id = blobmsg_get_u32(tb[CMD_ID]);
        blobmsg_add_u32(&b, "id", tmp_id);
    }

    if (tb[CMD_MSG])
    {
        tmp_msg = blobmsg_get_string(tb[CMD_MSG]);
        blobmsg_add_string(&b, "msg", tmp_msg);
    }

    ubus_send_reply(ctx, req, b.head);

    return 0;
}

static const struct ubus_method station_methods[] = {
    UBUS_METHOD("echo", station_echo, cmd_policy),
};

static struct ubus_object_type station_object_type =
    UBUS_OBJECT_TYPE("basicstation", station_methods);

static struct ubus_object station_object = {
    .name		= "basicstation",
    .type		= &station_object_type,
    .methods	= station_methods,
    .n_methods	= ARRAY_SIZE(station_methods)
};

static void station_ubus_ref_inc()
{
	ctx_ref++;
}

static void station_ubus_ref_dec()
{
	ctx_ref--;
	if (!ctx)
		return;

	if (ctx_ref)
		return;

	aio_t * aio_ctx = aio_fromCtx(ctx);
	aio_close(aio_ctx);
	ubus_free(ctx);
	ctx = NULL;
}

static void station_ubus_connection_lost(struct ubus_context *ctx)
{
	aio_t * aio_ctx = aio_fromCtx(ctx);
	aio_close(aio_ctx);

	// TODO: add a timer for reconnection
}

static void ubus_receive(struct aio* ah)
{
	struct ubus_context *ctx = ah->ctx;
	ubus_handle_event(ctx);
}


static bool station_ubus_init(void)
{
	if (ctx)
		return true;

	ctx = ubus_connect(NULL);
	if (!ctx)
		return false;

	ctx->connection_lost = station_ubus_connection_lost;
    aio_open(ctx, ctx->sock.fd, ubus_receive, NULL);
	return true;
}

void station_ubus_add()
{
	int ret;
	if (!station_ubus_init())
		return;

	ret = ubus_add_object(ctx, &station_object);

	if(ret)
		return;

	station_ubus_ref_inc();
}

void station_ubus_free()
{
	if (!ctx)
	{
		return;
	} else {
		ubus_remove_object(ctx, &station_object);

		station_ubus_ref_dec();
	}

}