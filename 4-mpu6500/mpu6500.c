#include <debug.h>

#include <nuttx/config.h>
#include <nuttx/kmalloc.h>
#include <nuttx/fs/fs.h>
#include <nuttx/i2c/i2c_master.h>

#if defined (CONFIG_I2C) && defined(CONFIG_SENSORS_MPU6500)

#define MPU6500_WHO_AM_I 0x75

int mpu6500_register(FAR const char *devpath, FAR struct i2c_master_s *i2c);
static int mpu6500_open(FAR struct file *filep);
static int mpu6500_close(FAR struct file *filep);
static off_t mpu6500_ioctl(FAR struct file *filep, off_t cmd, int arg);
static ssize_t mpu6500_read(FAR struct file *filep, FAR char *buffer, size_t buflen);

struct mpu6500_dev_s
{
    FAR struct i2c_master_s *i2c;
    uint8_t addr;
    int freq;
};

static const struct file_operations mpu6500_fops = {
    mpu6500_open,
    mpu6500_close,
    mpu6500_read,
    NULL,
    mpu6500_ioctl,
    NULL,
#ifndef CONFIG_DISABLE_POLL
    NULL,
#endif
    NULL
};

int mpu6500_register(FAR const char *devpath, FAR struct i2c_master_s *i2c)
{
    FAR struct mpu6500_dev_s *priv; /* private data */
    int ret;

    priv = (FAR struct mpu6500_dev_s *)kmm_malloc(sizeof(struct mpu6500_dev_s));
    if(!priv) {
        snerr("ERROR: Failed to allocate instance\n");
        return -ENOMEM;
    }

    priv->i2c = i2c;
    priv->addr = 0; //TODO
    priv->freq = 0; //TODO

    ret = register_driver(devpath, &mpu6500_fops, 0666, priv);
    if(ret < 0) {
        snerr("ERROR: Failed to register driver: %d\n", ret);
        kmm_free(priv);
    }

    return ret;
}

static int mpu6500_open(FAR struct file *filep)
{
    return OK;
}

static int mpu6500_close(FAR struct file *filep)
{
    return OK;
}

static off_t mpu6500_ioctl(FAR struct file *filep, off_t cmd, int arg)
{
    return OK;
}

static ssize_t mpu6500_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
    return OK;
}

static uint8_t mpu6500_getreg8(FAR struct mpu6500_dev_s *priv, uint8_t regaddr)
{
    uint8_t ret;
    uint8_t regval;

    struct i2c_config_s config = {
        .frequency = priv->freq,
        .address = priv->addr,
        .addrlen = 7
    };

    /* Write register address */
    ret = i2c_write(priv->i2c, &config, &regaddr, 1);
    if(ret < 0) {
        snerr("ERROR: i2c_write failed: %d\n", ret);
        return ret;
    }

    /* Read register value */
    ret = i2c_read(priv->i2c, &config, &regval, 1);
    if(ret < 0) {
        snerr("ERROR: i2c_read failed: %d\n", ret);
        return ret;
    }

    return regval;
}

static void mpu6500_putreg8(FAR struct mpu6500_dev_s *priv, uint16_t regaddr, uint16_t regval)
{
    struct i2c_config_s config = {
        .frequency = priv->freq,
        .address = priv->addr,
        .addrlen = 7
    };

    uint8_t data[2];
    data[0] = regaddr;
    data[1] = regval;

    i2c_write(priv->i2c, &config, (uint8_t *) &data, 2);
}

#endif
