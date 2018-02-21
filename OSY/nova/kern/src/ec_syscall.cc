#include "ec.h"
#include "ptab.h"

typedef enum {
    sys_print      = 1,
    sys_sum        = 2,
    sys_break      = 3,
    sys_thr_create = 4,
    sys_thr_yeild  = 5,
} Syscall_numbers;

void free_block(mword to,mword from){
    void *p;
    while(from > to){

        if((from & ~PAGE_MASK)==(to & ~PAGE_MASK) && (to&PAGE_MASK)==0){
            break;
        }if(from - to< PAGE_MASK && (from&PAGE_MASK)==0){
            break;
        }if((from & PAGE_MASK)==0){
            from-=PAGE_SIZE;
        }

        p = Kalloc::phys2virt(Ptab::get_mapping(from) & ~PAGE_MASK);
        Ptab::insert_mapping(from,Kalloc::virt2phys(p),0);
        printf("freeing: 0x%lx\n",from & ~PAGE_MASK);
        Kalloc::allocator.free_page(p);
        if((from & PAGE_MASK)!=0){
            from-=PAGE_SIZE;
        }
    }
}

void Ec::syscall_handler (uint8 a)
{
    // Get access to registers stored during entering the system - see
    // entry_sysenter in entry.S
    Sys_regs * r = current->sys_regs();
    Syscall_numbers number = static_cast<Syscall_numbers> (a);

    switch (number) {
        case sys_print: {
            char *data = reinterpret_cast<char*>(r->esi);
            unsigned len = r->edi;
            for (unsigned i = 0; i < len; i++)
                printf("%c", data[i]);
            break;
        }
        case sys_break: {
            r->eax = Ec::break_current;
            //printf("pagemask: 0x%lx\n",PAGE_MASK);
            if(r->esi==0 || r->esi==Ec::break_current){
                break;
            }
            if(r->esi < Ec::break_min || r->esi > 0xC0000000){
                r->eax=0;
                break;
            }
            if(Ec::break_current > r->esi){
                free_block(r->esi,Ec::break_current);
                Ec::break_current=r->esi;
                break;
            }if(Ec::break_current < r->esi){
                mword tmp = Ec::break_current;
                while(tmp < r->esi){

                    if((r->esi & ~PAGE_MASK)==(tmp & ~PAGE_MASK) && (tmp&PAGE_MASK)!=0){
                        break;
                    }
                    if(r->esi-tmp < PAGE_MASK && (r->esi&PAGE_MASK)==0){
                        break;
                    }
                    if((tmp & PAGE_MASK)!=0){
                        tmp+=PAGE_SIZE;
                    }
                    void *p;
                    p = Kalloc::allocator.alloc_page(1,Kalloc::FILL_0);
                    //printf("allocating %p\n",p);
                    if((!p || !Ptab::insert_mapping(tmp,Kalloc::virt2phys(p),Ptab::PRESENT | Ptab::RW | Ptab::USER))){
                        free_block(Ec::break_current,tmp);
                        r->eax=0;
                        goto END;
                    }
                    printf("allocating 0x%lx\n",tmp & ~PAGE_MASK);
                    if((tmp & PAGE_MASK)==0){
                        tmp+=PAGE_SIZE;
                    }
                }
                Ec::break_current=r->esi;
            }
            END:
            break;
        }
        default:
            printf ("unknown syscall %d\n", number);
            break;
    };

    ret_user_sysexit();
}
