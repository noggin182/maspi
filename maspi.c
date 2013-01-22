// -----------------------------------------------------------------------------------
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
// -----------------------------------------------------------------------------------
static struct spi_board_info pseudo_spi_device =
{
  .modalias     = "spidev",
  .max_speed_hz = 500000,
  .bus_num      = 0,
  .chip_select  = 2,
  .mode         = SPI_MODE_0 | SPI_NO_CS
};
// -----------------------------------------------------------------------------------
static struct spi_device *maspi_device = NULL;
// -----------------------------------------------------------------------------------
static int __init init_maspi(void)
{
  struct spi_master *master;
  printk(KERN_INFO "registering maspi spi device\n");
  master = spi_busnum_to_master(0);
  if (!master)
  {
    printk(KERN_ALERT "could not find master spi driver");
    return -1;
  }
  maspi_device = spi_new_device(master, &pseudo_spi_device);
  if (!maspi_device)
  {
    printk(KERN_ALERT "could not add extra spi device [CS2]");
    return -1;
  }
  return 0;
}
// -----------------------------------------------------------------------------------
static void __exit exit_maspi(void)
{
  if (maspi_device)
  {
    printk(KERN_INFO "unregistering maspi spi device\n");
    spi_unregister_device(maspi_device);
  }
}
// -----------------------------------------------------------------------------------
module_init(init_maspi);
module_exit(exit_maspi);
// -----------------------------------------------------------------------------------
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noggin182 (Matthew Gregory) <rpi-guy@nogsnet.com>");
MODULE_DESCRIPTION("Exposes an extra spi slave with no chip select");
MODULE_VERSION("0.3");
// -----------------------------------------------------------------------------------
